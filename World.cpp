//
// Created by Kiani on 16/12/2021.
//

#include "World.h"


void World::generateRandomEntities()
{
        float platformWidth = entities.back().first->getWidth();
        float platformHeight = entities.back().first->getHeight();
        std::shared_ptr<Random> random = Random::getInstance();
        std::pair<float, float> lastPlatformPosition = entities.back().first->getPosition();
        while (!camera.screenFilled(lastPlatformPosition)) {
                // We will produce platforms (and bonuses) as long as last platform is not at top of view
                lastPlatformPosition = entities.back().first->getPosition();
                float newPlatformY =
                    lastPlatformPosition.second + random->randomPlatformY(platformHeight, minPlatformDistance);
                float newPlatformX = random->randomPlatformX(platformWidth);
                std::unique_ptr<PlatformModel> newPlatform =
                    factory->createPlatform(newPlatformX, newPlatformY, random->randomPlatformType());
                // we will also have a chance of generating a bonus or enemy after the platform has been created
                BonusType::Type bonustype = random->randomBonusType();
                EnemyType::Type enemyType = random->randomEnemyType();
                if ((bonustype != BonusType::none) && (newPlatform->getType() != PlatformType::temporaryP)) {
                        std::unique_ptr<BonusModel> newBonus = factory->createBonus(newPlatform, bonustype);
                        entities.emplace_back(std::move(newPlatform), std::move(newBonus));
                } else if((enemyType != EnemyType::none) && (newPlatform->getType() != PlatformType::temporaryP)){
                        std::unique_ptr<EnemyModel> newEnemy = factory->createEnemy(newPlatform, enemyType);
                        entities.emplace_back(std::move(newPlatform), std::move(newEnemy));
                }
                else {
                        entities.emplace_back(std::move(newPlatform), nullptr);
                }
        }
}

void World::drawEntities()
{
        for (auto& entity : entities) {
                entity.first->drawEntity(camera); // draw platforms
                if (entity.second != nullptr) {
                        // draw bonuses
                        entity.second->drawEntity(camera);
                }
        }
        if (activeBonus != nullptr) {
                // needs to be drawn after platforms/bonuses
                activeBonus->drawEntity(camera);
        }
        for(auto& bullet: friendlyBullets){
                bullet->drawEntity(camera);
        }
}

void World::drawPlayer() { player->drawEntity(camera); }

bool World::movePlayer()
{
        player->movePlayer();
        updateActiveBonus(); // will updateJetpack the position of the jetpack
        if (!checkPlayerInView()) {
                player->dead();
                // game over
                return false;
        }
        std::pair<bool, int> cameraUpdate = camera.updateMaxHeight(player->getPosition());
        // returns true if the camera has moved (player passed half screen) and int value of how much the camera moved
        if (cameraUpdate.first) {
                player->increaseScore(cameraUpdate.second);
                checkEntitiesInView(); // if the camera has moved we have to destruct platforms that have gone out of
                                       // view
        };
        return true;
}

void World::checkEntitiesInView()
{
        for (auto it = entities.begin(); it != entities.end();) {
                if (camera.checkView((*it).first->getPosition())) {
                        // we check if the platform is still in view
                        it++;
                } else {
                        it = entities.erase(it);
                        // this will delete both the platform and the attached bonus
                }
        }
        for (auto it = backgroundTiles.begin(); it != backgroundTiles.end();) {
                if (camera.checkView((*it).back()->getPosition())) {
                        // we check if the position of a tile in a row is in view
                        it++;
                } else {
                        it = backgroundTiles.erase(it);
                        // if it is out of view we delete the whole row
                }
        }
        for(auto it = friendlyBullets.begin(); it != friendlyBullets.end();){
                if (camera.checkView((*it)->getPosition())) {
                        // we check if the position of a bullet is on screen
                        it++;
                } else {
                        it = friendlyBullets.erase(it);
                        // if it is out of view we delete the bullet
                }
        }

}

void World::moveEntities()
{
        for (auto& entity : entities) {
                entity.first->movePlatform();
                // first we move the platform
                if (entity.second != nullptr) {
                        entity.second->moveEntityOnPlatform(*entity.first);
                        // will updateJetpack bonus position according to platformpos
                }
        }
        for( auto& bullet: friendlyBullets){
                bullet->move();
        }
}

bool World::checkPlatformCollision(std::pair<float, float> entityPosition, float entityWidth, float entityHeight)
{
        if (player->getCurrentSpeed() > 0.f) {
                // if player is jumping we do not need to check platform collision
                return false;
        }
        std::pair<float, float> playerPosition = player->getPosition(); // centre of the player
        float precision = 0.015f;
        if (player->getCurrentSpeed() >= -0.5f) {
                precision = 0.005f;
        }
        if (entityPosition.second <= playerPosition.second) {
                // platform is below player
                std::pair<float, float> platformUpperLeftCorner{0, 0};
                std::pair<float, float> platformUpperRightCorner{0, 0};
                std::pair<float, float> playerLowerLeftCorner{0, 0};
                std::pair<float, float> playerLowerRightCorner{0, 0};
                // this is to calc LOWER corners of rectanglebox of player
                float playerWidth = player->getWidth();
                float playerHeight = player->getHeight();
                playerLowerLeftCorner.first = playerPosition.first - playerWidth;
                playerLowerLeftCorner.second = playerPosition.second - playerHeight;
                playerLowerRightCorner.first = playerPosition.first + playerWidth;
                playerLowerRightCorner.second = playerLowerLeftCorner.second;
                // this is to calc UPPER corners of rectanglebox of platforms/bonusses
                platformUpperLeftCorner.first = entityPosition.first - entityWidth;
                // entityPosition is  the centre of the entity
                platformUpperLeftCorner.second = entityPosition.second + entityHeight;
                // need to add/subtract width and height to get rectangle corners
                platformUpperRightCorner.first = entityPosition.first + entityWidth;
                platformUpperRightCorner.second = platformUpperLeftCorner.second;
                // now we check collision between player and this entity
                if ((playerLowerLeftCorner.second - platformUpperLeftCorner.second) <= precision &&
                    (playerLowerLeftCorner.second - platformUpperLeftCorner.second) >= -precision) {
                        // player is inside platform on Y-axis ; check if distance is negative (with varrying precision)
                        if (playerLowerRightCorner.first >= platformUpperLeftCorner.first &&
                            playerLowerLeftCorner.first <= platformUpperRightCorner.first) {

                                return true;
                                // player is not on left or right of the entity
                                // collision
                        } else {
                                return false;
                        }
                }
        }
        return false;
}

bool World::checkPlayerInView()
{
        std::pair<float, float> playerpos = player->getPosition();
        playerpos.second = playerpos.second - player->getHeight() - 0.1f; //-0.1 to counter the margin for platforms
        if (camera.checkView(playerpos)) {                                // player is in view
                return true;
        } else { // player has reached the bottom of the screen
                return false;
        }
}

void World::setPlayerDirection(PlayerMovement::Direction direction) { player->setPlayerDirection(direction); }

bool World::checkBonusCollision(const std::unique_ptr<BonusModel>& bonus)
{
        std::pair<float, float> bonusUpperLeftCorner{0, 0};
        std::pair<float, float> bonusUpperRightCorner{0, 0};
        std::pair<float, float> bonusLowerLeftCorner{0, 0};
        std::pair<float, float> bonusLowerRightCorner{0, 0};
        std::pair<float, float> playerLowerLeftCorner{0, 0};
        std::pair<float, float> playerLowerRightCorner{0, 0};
        std::pair<float, float> playerUpperLeftCorner{0, 0};
        std::pair<float, float> playerUpperRightCorner{0, 0};
        // this is to calc LOWER corners of rectanglebox of player
        std::pair<float, float> playerPosition = player->getPosition();
        float playerWidth = player->getWidth();
        float playerHeight = player->getHeight();
        playerLowerLeftCorner.first = playerPosition.first - playerWidth;
        playerLowerLeftCorner.second = playerPosition.second - playerHeight;
        playerLowerRightCorner.first = playerPosition.first + playerWidth;
        playerLowerRightCorner.second = playerLowerLeftCorner.second;
        playerUpperLeftCorner.first = playerLowerLeftCorner.first;
        playerUpperRightCorner.first = playerLowerRightCorner.first;
        playerUpperLeftCorner.second = playerPosition.second + playerHeight;
        playerUpperRightCorner.second = playerUpperLeftCorner.second;
        // we now have the full rectangleCollision box of the player
        // now we calc the rectanglebox of bonusses
        std::pair<float, float> bonusPosition = bonus->getPosition();
        float bonusWidth = bonus->getWidth();
        float bonusHeight = bonus->getHeight();
        bonusUpperLeftCorner.first = bonusPosition.first - bonusWidth;
        // entityPosition is  the centre of the entity
        bonusUpperLeftCorner.second = bonusPosition.second + bonusHeight;
        // need to add/subtract width and height to get rectangle corners
        bonusUpperRightCorner.first = bonusPosition.first + bonusWidth;
        bonusUpperRightCorner.second = bonusUpperLeftCorner.second;
        bonusLowerLeftCorner.first = bonusUpperLeftCorner.first;
        bonusLowerRightCorner.first = bonusUpperRightCorner.first;
        bonusLowerRightCorner.second = bonusPosition.second - bonusHeight;
        bonusLowerLeftCorner.second = bonusLowerRightCorner.second;
        if (playerLowerLeftCorner.second <= bonusUpperRightCorner.second &&
            playerUpperLeftCorner.second >= bonusLowerRightCorner.second) {
                // if this is true then the player is NOT above/below the bonus entirely
                if (playerLowerLeftCorner.first <= bonusUpperRightCorner.first &&
                    playerLowerRightCorner.first >= bonusUpperLeftCorner.first) {
                        // if this is true then the player is inside the bonus
                        return true;
                }
        } else {
                return false;
        }
        return false;
}

bool World::globalCollision(EntityModel& livingEntity, EntityModel& staticEntity){
        std::pair<float, float> staticEntityUpperLeftCorner{0, 0};
        std::pair<float, float> staticEntityUpperRightCorner{0, 0};
        std::pair<float, float> staticEntityLowerLeftCorner{0, 0};
        std::pair<float, float> staticEntityLowerRightCorner{0, 0};
        std::pair<float, float> livingEntityLowerLeftCorner{0, 0};
        std::pair<float, float> livingEntityLowerRightCorner{0, 0};
        std::pair<float, float> livingEntityUpperLeftCorner{0, 0};
        std::pair<float, float> livingEntityUpperRightCorner{0, 0};
        // this is to calc LOWER corners of rectanglebox of livingEntity
        std::pair<float, float> livingEntityPos = livingEntity.getPosition();
        float livingEntityWidth = livingEntity.getWidth();
        float livingEntityHeight = livingEntity.getHeight();
        livingEntityLowerLeftCorner.first = livingEntityPos.first - livingEntityWidth;
        livingEntityLowerLeftCorner.second = livingEntityPos.second - livingEntityHeight;
        livingEntityLowerRightCorner.first = livingEntityPos.first + livingEntityWidth;
        livingEntityLowerRightCorner.second = livingEntityLowerLeftCorner.second;
        livingEntityUpperLeftCorner.first = livingEntityLowerLeftCorner.first;
        livingEntityUpperRightCorner.first = livingEntityLowerRightCorner.first;
        livingEntityUpperLeftCorner.second = livingEntityPos.second + livingEntityHeight;
        livingEntityUpperRightCorner.second = livingEntityUpperLeftCorner.second;
        // we now have the full rectangleCollision box of the player
        // now we calc the rectanglebox of bonusses
        std::pair<float, float> staticEntityPosition = staticEntity.getPosition();
        float staticEntityWidth = staticEntity.getWidth();
        float staticEntityHeight = staticEntity.getHeight();
        staticEntityUpperLeftCorner.first = staticEntityPosition.first - staticEntityWidth;
        // entityPosition is  the centre of the entity
        staticEntityUpperLeftCorner.second = staticEntityPosition.second + staticEntityHeight;
        // need to add/subtract width and height to get rectangle corners
        staticEntityUpperRightCorner.first = staticEntityPosition.first + staticEntityWidth;
        staticEntityUpperRightCorner.second = staticEntityUpperLeftCorner.second;
        staticEntityLowerLeftCorner.first = staticEntityUpperLeftCorner.first;
        staticEntityLowerRightCorner.first = staticEntityUpperRightCorner.first;
        staticEntityLowerRightCorner.second = staticEntityPosition.second - staticEntityHeight;
        staticEntityLowerLeftCorner.second = staticEntityLowerRightCorner.second;
        //precision softens the collision check by allowing it to be detected a little earlier when falling
        float precision = 0.015;
        //increase precision if collisionEntity is going slow
        if (livingEntity.getCurrentSpeed() >= -0.5f) {
                precision = 0.005f;
        }
        if (livingEntityLowerLeftCorner.second <= (staticEntityUpperRightCorner.second + precision) &&
            livingEntityUpperLeftCorner.second >= staticEntityLowerRightCorner.second) {
                // if this is true then the livingEntity is NOT above/below the staticEntity entirely
                if (livingEntityLowerLeftCorner.first <= staticEntityUpperRightCorner.first &&
                    livingEntityLowerRightCorner.first >= staticEntityUpperLeftCorner.first) {
                        // if this is true then the livingEntity is inside of the staticEntity
                        return staticEntity.collide(livingEntity);

                }
        } else {
                return false;
        }
        return false;
}

void World::checkCollision()
{
        // no need to check collision if player has 0 hp
        if(player->isDead()){
                return;
        }

        for (auto it = entities.begin(); it != entities.end();) {
                if ((*it).second != nullptr) {
                        // if there is a bonus, check bonuscollision first
                        if(globalCollision(*player, *(*it).second)){
                                if((*it).second->getType() == BonusType::jetpack){
                                        activeBonus = std::move((*it).second);
                                        //safety
                                        (*it).second = nullptr;
                                        // the world will keep track of the activeBonus
                                        // platform no longer holds any bonus
                                }
                                else if (!((*it).second->getType() == BonusType::spring ||(*it).second->getType() == BonusType::spike)){
                                        //delete bonus
                                        (*it).second.reset();
                                        //safety
                                        (*it).second = nullptr;
                                        // platform no longer holds any bonus
                                }
                        }
                }
                if(globalCollision(*player,*(*it).first)){
                        player->platformCollide(*(*it).first);
                        if((*it).first->getType() == PlatformType::temporaryP){
                                it = entities.erase(it);
                        }

                }

//                                if (checkPlatformCollision(bonusPos, bonusWidth, bonusHeight)) {
//                                        // spring/spike bonus collision works the same as a platform collision
//                                        player->platformCollide((*it).first);
//                                        // this will give player the normal jump speed and notify the player which
//                                        // platform the bonus is on
//                                        (*it).second->applyToPlayer(player);
//                                        // this will apply the (spring)bonus and increase score
//                                }
//                        } else { // its not a spring/spike bonus
//                                if (checkBonusCollision((*it).second) && activeBonus == nullptr) {
//                                        // player picks up bonus and there is no bonus active
//                                        //player->platformCollide((*it).first);
//                                        // this will give player the normal jump speed and notfy the player which
//                                        // platform the bonus is on
//
//                                        // this will apply the bonus
//                                        (*it).second->applyToPlayer(player);
//
//                                        if((*it).second->getType() == BonusType::jetpack){
//                                                activeBonus = std::move((*it).second);
//                                                // the world will keep track of the activeBonus
//                                        }
//                                        else{
//                                                (*it).second.reset();
//                                        }
//                                        (*it).second = nullptr;
//                                        // platform no longer holds any bonus
//                                }
//                        }
//                }
//                if (checkPlatformCollision(platformPos, platformWidth, platformHeight)) {
//                        // if there is collision between platform and player
//                        player->platformCollide((*it).first);
//                        //delete temporaryPlatform on collision
//                        if ((*it).first->getType() == PlatformType::temporaryP) {
//                                it = entities.erase(it);
//                        }
//                        //teleport platform horizontally/vertically on collision
//                        else if((*it).first->getType() == PlatformType::horizontalPteleport
//                                 ||(*it).first->getType() == PlatformType::verticalPteleport ){
//                                teleportPlatform((*it).first);
//                        }

                it++;
        }
}

void World::updateActiveBonus()
{
        // is used to updateJetpack the position of the active bonus and destruct it once it is expired
        if (activeBonus == nullptr) {
                return;
        } else {
                if (activeBonus->updateJetpack(*player)) {
                        // bonus has not yet expired
                } else {
                        // bonus has expired
                        activeBonus.reset();
                        activeBonus = nullptr;
                }
        }
}

void World::generateBackground()
{
        while (!camera.screenFilled(backgroundTiles.back().back()->getPosition())) {
                // checks if the screen is filled with tiles
                float lastrowY = backgroundTiles.back().back()->getPosition().second;
                float newRowY = lastrowY + backgroundTiles.back().back()->getHeight() + 0.001;
                // little bit of margin so they don't overlap perfectly(can cause visual rounding errors otherwise)
                backgroundTiles.push_back(makeBackgroundRow(newRowY));
        }
}

std::list<std::unique_ptr<BGTileModel>> World::makeBackgroundRow(float currentRowY)
{
        float currentRowX = -1; // world view is defined [-1,1];
        std::list<std::unique_ptr<BGTileModel>> row;
        while (currentRowX < 1) {
                row.push_back(factory->createBGTile(currentRowX, currentRowY));
                currentRowX += row.back()->getWidth();
                // we need to add the width to make a proper grid
        }
        return row;
}

void World::drawBackground()
{
        for (auto& row : backgroundTiles) {
                for (auto& tile : row) {
                        tile->drawEntity(camera);
                }
        }
}
void World::shootFriendlyBullet() {
        std::pair<float,float> topCentrePlayer = std::pair<float,float>(player->getPosition());
        topCentrePlayer.first += player->getWidth();
        topCentrePlayer.second += player->getHeight();
        friendlyBullets.push_back(factory->createBullet(topCentrePlayer.first, topCentrePlayer.second));

}
