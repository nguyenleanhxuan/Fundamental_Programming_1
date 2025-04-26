/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class Unit;
class UnitList;
class Army;
class TerrainElement;

class Vehicle;
class Infantry;

class LiberationArmy;
class ARVN;

class Position;

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;

class BattleField;

class HCMCampaign;
class Configuration;

enum VehicleType
{
    TRUCK,
    MORTAR,
    ANTIAIRCRAFT,
    ARMOREDCAR,
    APC,
    ARTILLERY,
    TANK
};
enum InfantryType
{
    SNIPER,
    ANTIAIRCRAFTSQUAD,
    MORTARSQUAD,
    ENGINEER,
    SPECIALFORCES,
    REGULARINFANTRY
};

class Army
{
protected:
    int LF, EXP;
    string name;
    UnitList *unitList;
    BattleField *battleField;

public:
    Army(Unit** unitArray, int size, const string& name, BattleField* battleField);
    // PHỤC VỤ CHO 4.1
    UnitList* getUnitList() const { return unitList; }
    int getLF() const { return LF; }
    int getEXP() const { return EXP; }
    // HẾT

    virtual void fight(Army *enemy, bool defense = false) = 0;
    virtual string str() const = 0;
    
    // PHỤC VỤ CHO BÀI 7
    string getName() const { return name; }  
    void decreaseAttackScore(int delta) ;
    void increaseAttackScore(int delta) ;
    // HẾT PHỤC VỤ B7
    // PHỤC VỤ B10
    void removeWeakUnits(); 
    virtual void prepareForBattle(); 
};

class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0);
    Position(const string &str_pos); // Example: str_pos = "(1,15)"
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const; // Example: returns "(1,15)"
    double distanceTo(const Position &other) const; // PHỤC VỤ BÀI 7

};

class Unit
{
protected:
    int quantity, weight;
    Position pos;
    int attackScore; // phục vụ b7

public:
    Unit(int quantity, int weight, Position pos);
    virtual ~Unit();
    virtual int getAttackScore() = 0;
    Position getCurrentPosition() const;
    virtual string str() const = 0;
    virtual void decreaseQuantity(int percent) {
        quantity -= quantity * percent / 100;
        if (quantity < 0) quantity = 0;  // Không để âm
    }
    int getQuantity() const {
        return quantity;
    }

    void setQuantity(int newQuantity) {
        quantity = newQuantity;
    }

    void decreaseWeight(int percent) {
        weight -= weight * percent / 100;
        if (weight < 0) weight = 0;  
    }

    //PHỤC VỤ BÀI 7
    void decreaseAttackScore(int amount) {
        attackScore -= amount;
        if (attackScore < 0) attackScore = 0;  // Không để attackScore âm
    }

    void increaseAttackScore(int amount) {
        attackScore += amount;
        if (attackScore < 0) attackScore = 0;  // Không để attackScore âm
    }

    virtual void setAttackScore(int newScore) {
        attackScore = newScore;
    }
    // HẾT PV B7
};

// BÀI 5
struct Node {
    Unit* unit;  // Con trỏ tới đơn vị
    Node* next;  // Con trỏ tới node tiếp theo

    Node(Unit* unit) : unit(unit), next(nullptr) {}
};

class UnitList
{
private:
    int capacity;
    vector<Unit*> units; 
    // TODO
    Node* head;    // Con trỏ đầu danh sách
    int vehicleCount;  // Số lượng phương tiện
    int infantryCount; // Số lượng bộ binh
public:
    // PHỤC VỤ 4.1 -- Phương thức trả về tất cả các đơn vị trong danh sách
    vector<Unit*> getAllUnits() const {
        vector<Unit*> units;
        Node* current = head;
        while (current != nullptr) {
            units.push_back(current->unit);
            current = current->next;
        }
        return units;
    }
    //HẾT PHỤC VỤ 4.1
    UnitList( int LF, int EXP);
    bool insert(Unit *unit);                   // return true if insert successfully
    bool isContain(VehicleType vehicleType);   // return true if it exists
    bool isContain(InfantryType infantryType); // return true if it exists
    string str() const;
    // TODO
    int size() const;                          // Thêm phương thức size()
    Unit* get(int index) const;                // Thêm phương thức get()
    void remove(Unit* unit);                   // Thêm phương thức remove()
};


// BÀI 8
class BattleField
{
private:
    int n_rows, n_cols;
    // TODO
    TerrainElement*** terrain; // ma trận 2 chiều động
    
        void setTerrainAt(Position& pos, TerrainElement* element);

    public:
    BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
    ~BattleField();

    // Thêm phương thức str() để trả về thông tin của BattleField
    string str() const {
        return "BattleField[n_rows=" + to_string(n_rows) + ", n_cols=" + to_string(n_cols) + "]";
    }
    TerrainElement* getTerrainAt(int row, int col) const;
};


// BÀI 2
class Vehicle : public Unit
{
private:
    VehicleType vehicleType;  // Loại phương tiện chiến đấu

public:
    // Constructor
    Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType);

    // Phương thức tính điểm đóng góp của phương tiện
    int getAttackScore() override;

    // Phương thức trả về chuỗi mô tả phương tiện
    string str() const override;

    // PHỤC VỤ BÀI 5
    VehicleType getVehicleType() const;

    // PHỤC VỤ BÀI 7
    void setAttackScore(int newScore) override {
        attackScore = newScore;
    }
};

// BÀI 3
class Infantry : public Unit
{
private:
    InfantryType infantryType; // Loại bộ binh

public:
    // Constructor
    Infantry(int quantity, int weight, const Position pos, InfantryType infantryType);

    // Phương thức tính điểm đóng góp của bộ binh
    int getAttackScore() override;

    // Phương thức trả về chuỗi mô tả bộ binh
    string str() const override;

    // PHỤC VỤ BÀI 5
    // Thêm phương thức getInfantryType
    InfantryType getInfantryType() const;

    // PHỤC VỤ BÀI 7
    void setAttackScore(int newAttackScore) {
        attackScore = newAttackScore;
    }
};


// BÀI 4.1
class LiberationArmy : public Army
{
public:
    LiberationArmy(Unit** unitArray, int size, const string& name, BattleField* battleField);
    void fight(Army *enemy, bool defense = false) override;
    string str() const override;
};

// 4.2
class ARVN : public Army
{
public:
    ARVN(Unit** unitArray, int size, const string& name, BattleField* battleField);

    void fight(Army* enemy, bool defense = false) override;
    string str() const override;
};

// BÀI 7

class TerrainElement
{
protected:
    Position pos;
public:
    TerrainElement() ;
    
    virtual ~TerrainElement();

    // Phương thức thuần ảo để xác định ảnh hưởng của yếu tố địa hình đến quân đội
    virtual void getEffect(Army* army) = 0;
};

class Road : public TerrainElement {
public:
    Road(Position pos);   // Constructor
    ~Road();              // Destructor
    void getEffect(Army* army) override;
};

class Mountain : public TerrainElement {
private:
    Position position;
public:
    Mountain(Position pos) : TerrainElement() { this->pos = pos; }
    ~Mountain(){};
    void getEffect(Army* army) override;
};
    
class River : public TerrainElement {
private:
    Position position;
public:
    River(Position pos) : TerrainElement() { this->pos = pos; }
    ~River(){};
    void getEffect(Army* army) override;
};
    
class Urban : public TerrainElement {
private:
    Position position;
public:
    Urban(Position pos) : TerrainElement() { this->pos = pos; }
    ~Urban(){};
    void getEffect(Army* army) override;
};
    
class Fortification : public TerrainElement {
private:
    Position position;
public:
    Fortification(Position pos) : TerrainElement() { this->pos = pos; }
    ~Fortification(){};
    void getEffect(Army* army) override;
};
    
class SpecialZone : public TerrainElement {
private:

public:
    SpecialZone(Position pos) : TerrainElement() { this->pos = pos; };
    ~SpecialZone(){};
    void getEffect(Army* army) override;
};



// BÀI 9
class Configuration {
private:
    int num_rows, num_cols;
    vector<Position*> arrayForest, arrayRiver, arrayFortification, arrayUrban, arraySpecialZone;
    vector<Unit*> liberationUnits, ARVNUnits;
    int eventCode;

public:
    Configuration(const string& filepath);
    ~Configuration();

    string str() const;

    void parsePositionArray(const string& str, vector<Position*>& array);
    void parseUnitList(const string& str, vector<Unit*>& units, int armyBelong);
    int parseEventCode(const string& str);

    int getNumRows() const { return num_rows; }
    int getNumCols() const { return num_cols; }
    const vector<Position*>& getForestPositions() const { return arrayForest; }
    const vector<Position*>& getRiverPositions() const { return arrayRiver; }
    const vector<Position*>& getFortificationPositions() const { return arrayFortification; }
    const vector<Position*>& getUrbanPositions() const { return arrayUrban; }
    const vector<Position*>& getSpecialZonePositions() const { return arraySpecialZone; }
    
    const vector<Unit*>& getLiberationUnits() const { return liberationUnits; }
    int getLiberationUnitsSize() const { return liberationUnits.size(); }
    
    const vector<Unit*>& getARVNUnits() const { return ARVNUnits; }
    int getARVNUnitsSize() const { return ARVNUnits.size(); }
    
    int getEventCode() const { return eventCode; }
    
};


//BÀI 10
class HCMCampaign
{
private:
    Configuration *config;
    BattleField *battleField;
    LiberationArmy *liberationArmy;
    ARVN* arvnArmy;

public:
    HCMCampaign(const string &config_file_path);
    void run();
    string printResult();
};


#endif