/***************************************************
 *
 * Joseph Krambeer
 * This is for an abstract entity class that will serve
 * as the base for things like the player and enemies
 *
 ***************************************************/
#pragma once

namespace Game
{
    enum Disposition : Byte {ThePlayer=0, Allied=1, Neutral=2, Hostile=3};
    enum Status      : Byte {NoStatus=0, Poisoned=1, Asleep=2, Weakened=3 };

//-----Abstract Entity Class-----
    class Entity
    {
        private:
            Disposition disposition;
            Status      status;
            Byte        baseAttack;
            Byte        baseDefense;
            short       health;
            short       yCord;
            short       xCord;
            std::string name;
            cchar_t     entityIcon;

        public:
            Entity(Disposition disp,Status stat, Byte atk, Byte def, short health, short y,
                   short x, std::string name,cchar_t icon);

            Disposition  getDisposition();
            Status       getStatus();
            Byte         getBaseAttack();
            Byte         getBaseDefense();
            short        getHealth();
            short        getYCord();
            short        getXCord();
            std::string  getName();
            cchar_t      getIcon();

            void setCords(short y, short x);
            void setAtkAndDef(Byte atk, Byte def);
            void setStatus(Status newStatus);
            virtual void setHealth(short newHealth);

            virtual void moveEntity(int direction)=0;
    };//class Entity

    Entity::Entity(Disposition disp,Status stat, Byte atk, Byte def, short health, short y,
                   short x, std::string name,cchar_t icon)
    {
        this->disposition = disp;
        this->status      = stat;
        this->baseAttack  = atk;
        this->baseDefense = def;
        this->health      = health;
        this->yCord       = y;
        this->xCord       = x;
        this->name        = name;
        this->entityIcon  = icon;
    }//entity constructor

    void Entity::setAtkAndDef(Byte atk, Byte def)
    {
        this->baseAttack  = atk;
        this->baseDefense = def;
    }//setAtkAndDef

    void Entity::setCords(short y, short x)
    {
        this->yCord = y;
        this->xCord = x;
    }//setCords

    void Entity::setStatus(Status newStatus)
    {
        this->status = newStatus;
    }//setStatus

    void Entity::setHealth(short newHealth)
    {
        this->health = newHealth;
    }//setHealth

    Disposition Entity::getDisposition()
    {
        return this->disposition;
    }//getDisposition

    Status Entity::getStatus()
    {
        return this->status;
    }//getStatus

    Byte Entity::getBaseAttack()
    {
        return this->baseAttack;
    }//getBaseAttack

    Byte Entity::getBaseDefense()
    {
        return this->baseDefense;
    }//getBaseDefense

    short Entity::getHealth()
    {
        return this->health;
    }//getHealth

    short Entity::getYCord()
    {
        return this->yCord;
    }//getYCord

    short Entity::getXCord()
    {
        return this->xCord;
    }//getXCord

    std::string Entity::getName()
    {
        return this->name;
    }//getName

    cchar_t Entity::getIcon()
    {
        return this->entityIcon;
    }//getIcon

}//namespace Game
