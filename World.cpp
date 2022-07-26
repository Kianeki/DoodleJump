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
                entity.first->drawEntity(camera);
        }
        for (auto& entity : entities) {
                if (entity.second != nullptr) {
                        // draw bonuses and enemies
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
        for(auto& bullet: enemyBullets){
                bullet->drawEntity(camera);
        }
}

void World::drawPlayer() { player->drawEntity(camera); }

bool World::movePlayer()
{
        player->movePlayer();
        updateActiveBonus(); // will updateJetpack the position of the jetpack
        if (!checkPlayerInView()) {
                player->gameOver();
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
        for(auto it = enemyBullets.begin(); it != enemyBullets.end();){
                if (camera.checkView((*it)->getPosition())) {
                        // we check if the position of a bullet is on screen
                        it++;
                } else {
                        it = enemyBullets.erase(it);
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
                        // will update bonus/enemy position according to platformpos
                        entity.second->moveEntityOnPlatform(*entity.first);

                }
        }
        for( auto& bullet: friendlyBullets){
                bullet->move();
        }
        for( auto& bullet: enemyBullets){
                bullet->move();
        }
}


bool World::checkPlayerInView()
{
        std::pair<float, float> playerpos = player->getPosition();
        playerpos.second = playerpos.second - player->getHeight() - 0.2f; //-0.2 to counter the margin for platforms
        if (camera.checkView(playerpos)) {                                // player is in view
                return true;
        } else { // player has reached the bottom of the screen
                return false;
        }
}

void World::setPlayerDirection(PlayerMovement::Direction direction) { player->setPlayerDirection(direction); }



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
                        // if this is true then the livingEntity is inside the staticEntity
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
                        // first, check collision between player and entity(bonus/enemy) that is attached to platform
                        if(globalCollision(*player, *(*it).second)){
                                if((*it).second->getType() == BonusType::jetpack){
                                        activeBonus = std::move((*it).second);
                                        //safety
                                        (*it).second = nullptr;
                                        // the world will keep track of the activeBonus
                                        // platform no longer holds any bonus
                                }
                                //one time use bonuses get deleted on collision
                                else if ((*it).second->deleteOnCollision()){
                                        (*it).second.reset();
                                        //safety
                                        (*it).second = nullptr;
                                        // platform no longer holds any bonus
                                }
                        }
                }
                        //then, check collision between friendly bullets and entity(enemy) that is attached to platform
                        for(auto itBullets = friendlyBullets.begin(); itBullets != friendlyBullets.end();){
                                if((*it).second == nullptr) {
                                        // if entity is already deleted, we don't have to check further collision with bullets
                                     break;
                                }
                                if(globalCollision(*(*it).second,*(*itBullets))){
                                        // if a strong enemy is hit, it will shoot a bullet downwards at the player
                                        if((*it).second->getType() == EnemyType::strong){
                                                shootEnemyBullet(*(*it).second);
                                        }
                                        //if they collide then bullet must be deleted
                                        if((*itBullets)->deleteOnCollision()){
                                                itBullets = friendlyBullets.erase(itBullets);
                                        }

                                        // check if enemy is dead
                                        if((*it).second->isDead()){
                                                //if enemy is dead, award player
                                                int scoreIncrease = (*it).second->getType();
                                                player->increaseScore(scoreIncrease);
                                                //delete dead enemy
                                                (*it).second.reset();
                                                (*it).second = nullptr;
                                        }
                                }
                                itBullets++;
                }
                //then,check collision between player and platform
                if(globalCollision(*player,*(*it).first)){
                        (*it).first->fallingCollide(*player);
                        //delete temporary platform on collision
                        if((*it).first->deleteOnCollision()){
                                it = entities.erase(it);
                        }
                }
                it++;
        }
        for(auto it = enemyBullets.begin(); it != enemyBullets.end();){
                //collision between player and enemy bullet
                if(globalCollision(*player, *(*it))){
                        it = enemyBullets.erase(it);
                }
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
        if(reloaded){
                std::pair<float,float> topCentrePlayer = std::pair<float,float>(player->getPosition());
                topCentrePlayer.second += player->getHeight();
                friendlyBullets.push_back(factory->createBullet(topCentrePlayer.first, topCentrePlayer.second, BulletType::friendly));
        }
        reloaded = false;
}
void World::shootEnemyBullet(const EntityModel& enemy) {
        std::pair<float,float> bottomCentreEnemy = std::pair<float,float>(enemy.getPosition());
        bottomCentreEnemy.second -= enemy.getHeight();
        enemyBullets.push_back(factory->createBullet(bottomCentreEnemy.first, bottomCentreEnemy.second, BulletType::enemy));

}
void World::reload() { reloaded = true; }
World::World(std::unique_ptr<AbstractFactory> concreteFactory, unsigned int windowWidth, unsigned int windowHeight)
    : factory(std::move(concreteFactory)), camera(Camera(windowWidth, windowHeight))
{
        player = std::move(factory->createPlayer(0, -0.5)); // don't spawn too high or stuff gets wonky
        // place starter platform
        entities.emplace_back(std::move(factory->createPlatform(0, -0.7, PlatformType::staticP)), nullptr);
        // make bottom row of background
        backgroundTiles.push_back(makeBackgroundRow(-1));
        generateBackground();
        generateRandomEntities();
        drawEntities();
}
