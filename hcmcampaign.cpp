#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

// BÀI 1
// Constructor
Unit::Unit(int quantity, int weight, Position pos) : quantity(quantity), weight(weight), pos(pos) {}

// Destructor ảo
Unit::~Unit() {}

// Phương thức getCurrentPosition trả về vị trí hiện tại của đơn vị
Position Unit::getCurrentPosition() const {
    return pos;
}

// BÀI 2
// Constructor cho lớp Vehicle
Vehicle::Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType)
    : Unit(quantity, weight, pos), vehicleType(vehicleType) {}

// Phương thức getAttackScore tính điểm đóng góp của phương tiện
int Vehicle::getAttackScore()
{
    int typeValue = static_cast<int>(vehicleType);  // Lấy giá trị của loại phương tiện từ enum
    return ((typeValue * 304 + quantity * weight) / 30); 
}

string vehicleTypeToString(VehicleType type) {
    switch (type) {
        case TRUCK: return "TRUCK";
        case MORTAR: return "MORTAR";
        case ANTIAIRCRAFT: return "ANTIAIRCRAFT";
        case ARMOREDCAR: return "ARMOREDCAR";
        case APC: return "APC";
        case ARTILLERY: return "ARTILLERY";
        case TANK: return "TANK";
        default: return "UNKNOWN";
    }
}

string Vehicle::str() const {
    return "Vehicle[vehicleType=" + vehicleTypeToString(vehicleType) + ",quantity=" + to_string(quantity) +
           ",weight=" + to_string(weight) + ",position=" + pos.str() + "]";
}

//PHỤC VỤ CHO BÀI 5
VehicleType Vehicle::getVehicleType() const {
    return vehicleType;
}

// BÀI 3
// Constructor cho lớp Infantry
Infantry::Infantry(int quantity, int weight, const Position pos, InfantryType infantryType)
    : Unit(quantity, weight, pos), infantryType(infantryType) {}

// Phương thức getAttackScore 
int Infantry::getAttackScore()
{
    int typeValue = static_cast<int>(infantryType); 
    int score = typeValue * 56 + quantity * weight; 

    // Kiểm tra nếu là Lực lượng đặc biệt 
    if (infantryType == SPECIALFORCES && weight > 0 && sqrt(weight) == floor(sqrt(weight))) {
        score += 75;  
    }

    // Tính số cá nhân của năm 1975
    int personalNumber = (1975 + quantity + weight) % 9;
    if (personalNumber == 0) personalNumber = 9;

    // Điều chỉnh số lượng và tính lại score theo số cá nhân
    if (personalNumber > 7) {
        quantity = static_cast<int>(quantity * 1.2); 
    } else if (personalNumber < 3) {
        quantity = static_cast<int>(quantity * 0.9);  
    }

    return score;
}

// Phương thức str
string Infantry::str() const {
    string type_str;
    switch (infantryType) {
        case SNIPER: type_str = "SNIPER"; break;
        case ANTIAIRCRAFTSQUAD: type_str = "ANTIAIRCRAFTSQUAD"; break;
        case MORTARSQUAD: type_str = "MORTARSQUAD"; break;
        case ENGINEER: type_str = "ENGINEER"; break;
        case SPECIALFORCES: type_str = "SPECIALFORCES"; break;
        case REGULARINFANTRY: type_str = "REGULARINFANTRY"; break;
        default: type_str = "UNKNOWN"; break;
    }
    return "Infantry[infantryType=" + type_str + ",quantity=" + to_string(quantity) +
           ",weight=" + to_string(weight) + ",position=" + pos.str() + "]";
}

//PHỤC VỤ CHO BÀI 5
InfantryType Infantry::getInfantryType() const {
    return infantryType;
}

// BÀI 5
// Kiểm tra số có thể phân tích thành tổng các lũy thừa của cơ số k
bool isSpecialNumber(int num) {
    for (int k = 2; k < 10; ++k) {  // Kiểm tra các cơ số từ 2 đến 9
        int sum = 0;
        int temp = num;
        while (temp > 0) {
            sum += std::pow(temp % 10, k);  // Tính tổng các lũy thừa cơ số k
            temp /= 10;
        }
        if (sum == num) {
            return true;  // Nếu tìm thấy, trả về true
        }
    }
    return false;
}

// Constructor
UnitList::UnitList( int LF, int EXP) : head(nullptr), capacity(capacity), vehicleCount(0), infantryCount(0) {
    int totalScore = LF + EXP;
    // Kiểm tra số đặc biệt của tổng S
    if (isSpecialNumber(totalScore)) {
        this->capacity = 12;
    } else {
        this->capacity = 8;
    }
}

// BÀI 4

// hcmcampaign.cpp

// Phương thức decreaseAttackScore
void Army::decreaseAttackScore(int delta) {
    for (int i = 0; i < unitList->size(); ++i) {
        Unit* unit = unitList->get(i);
        unit->decreaseAttackScore(delta);  // Gọi phương thức decreaseAttackScore của Unit
    }
}

// Phương thức increaseAttackScore
void Army::increaseAttackScore(int delta) {
    for (int i = 0; i < unitList->size(); ++i) {
        Unit* unit = unitList->get(i);
        unit->increaseAttackScore(delta);  // Gọi phương thức increaseAttackScore của Unit
    }
}

// Phương thức removeWeakUnits
void Army::removeWeakUnits() {
    for (int i = 0; i < unitList->size(); ) {
        Unit* unit = unitList->get(i);
        if (unit->getAttackScore() <= 5) {
            unitList->remove(unit);
        } else {
            ++i;
        }
    }
}


Army::Army(Unit** unitArray, int size, const string& name, BattleField* battleField)
    : name(name), battleField(battleField), LF(0), EXP(0)
{
    int capacity = (isSpecialNumber(LF + EXP)) ? 12 : 8;
    
    unitList = new UnitList( LF, EXP);

    // Duyệt qua từng đơn vị để tính LF và EXP
    for (int i = 0; i < size; ++i) {
        unitList->insert(unitArray[i]);  // Thêm vào unitList

        // Kiểm tra loại đơn vị (Vehicle hay Infantry) để cộng điểm vào LF hoặc EXP
        if (dynamic_cast<const Vehicle*>(unitArray[i])) {
            LF += unitArray[i]->getAttackScore();
        } else if (dynamic_cast<const Infantry*>(unitArray[i])) {
            EXP += unitArray[i]->getAttackScore();
        }
    }
}

string Army::str() const
{
    return "Army[name=" + name + ",LF=" + to_string(LF) + ",EXP=" + to_string(EXP) + ",unitList=" + unitList->str() + ",battleField=" + battleField->str() + "]";
}

// BÀI 4.1
// Phương thức tính số Fibonacci gần nhất
int closestFibonacci(int n) {
    int a = 0, b = 1;
    while (b < n) {
        int temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

LiberationArmy::LiberationArmy(Unit** unitArray, int size, const string& name, BattleField* battleField)
    : Army(const_cast<Unit**>(unitArray), size, name, battleField) 
{
    unitList = new UnitList(LF, EXP);

    for (int i = 0; i < size; ++i) {
        unitList->insert(const_cast<Unit*>(unitArray[i]));
        Unit* nonConstUnit = const_cast<Unit*>(unitArray[i]);

        if (dynamic_cast<Vehicle*>(nonConstUnit)) {
            LF += nonConstUnit->getAttackScore();
        } else if (dynamic_cast<Infantry*>(nonConstUnit)) {
            EXP += nonConstUnit->getAttackScore();
        }
    }
}

void LiberationArmy::fight(Army *enemy, bool defense) {
    if (defense) {
        LF *= 1.3;
        EXP *= 1.3;
    } else {
        LF *= 1.5;
        EXP *= 1.5;
    }

    // Giảm số lượng quân trong trường hợp phòng thủ (không giao tranh)
    if (defense) {
        for (int i = 0; i < getUnitList()->size(); ++i) {
            Unit* unit = getUnitList()->get(i);
            unit->decreaseQuantity(10);  // Giảm 10% số lượng quân
        }
    } else {
        // Tìm tổ hợp A và B
        bool foundA = false, foundB = false;
        int totalInfantryScore = 0, totalVehicleScore = 0;
        vector<Unit*> selectedInfantry;
        vector<Unit*> selectedVehicle;

        // Tìm tổ hợp bộ binh (tổng điểm lớn hơn EXP của đối phương)
        for (Unit* unit : getUnitList()->getAllUnits()) {
            if (dynamic_cast<Infantry*>(unit)) {
                totalInfantryScore += unit->getAttackScore();
                selectedInfantry.push_back(unit);
                if (totalInfantryScore > enemy->getEXP()) {
                    foundA = true;
                    break;
                }
            }
        }

        // Tìm tổ hợp phương tiện (tổng điểm lớn hơn LF của đối phương)
        for (Unit* unit : getUnitList()->getAllUnits()) {
            if (dynamic_cast<Vehicle*>(unit)) {
                totalVehicleScore += unit->getAttackScore();
                selectedVehicle.push_back(unit);
                if (totalVehicleScore > enemy->getLF()) {
                    foundB = true;
                    break;
                }
            }
        }

        // Kiểm tra các điều kiện chiến thắng, hoà hoãn hoặc thất bại
        if (foundA && foundB) {
            // Chiến thắng, xoá các đơn vị khỏi danh sách
            for (auto& unit : selectedInfantry) {
                getUnitList()->remove(unit);  // Xóa đơn vị bộ binh khỏi danh sách
            }
            for (auto& unit : selectedVehicle) {
                getUnitList()->remove(unit);  // Xóa đơn vị phương tiện khỏi danh sách
            }
            // Cập nhật lại LF và EXP
            LF -= enemy->getLF();
            EXP -= enemy->getEXP();
            // Thêm quân của đối phương vào quân đội
            for (auto& unit : enemy->getUnitList()->getAllUnits()) {
                getUnitList()->insert(unit);  // Thêm vào danh sách nếu chưa có
            }
        } else {
            // Nếu không giao tranh xảy ra, giảm 10% trọng số của từng đơn vị
            for (int i = 0; i < getUnitList()->size(); ++i) {
                Unit* unit = getUnitList()->get(i);
                unit->decreaseWeight(10);  // Giảm trọng số của đơn vị
            }

            // Nếu Quân Giải phóng cần chi viện, tăng số lượng lên Fibonacci gần nhất
            if (LF < enemy->getLF() && EXP < enemy->getEXP()) {
                for (int i = 0; i < getUnitList()->size(); ++i) {
                    Unit* unit = getUnitList()->get(i);
                    int newQuantity = closestFibonacci(unit->getQuantity());
                    unit->setQuantity(newQuantity);  // Cập nhật lại số lượng
                }
            }
        }
    }
}


string LiberationArmy::str() const
{
    return "LiberationArmy[name=" + name + ",LF=" + to_string(LF) + ",EXP=" + to_string(EXP) + ",unitList=" + unitList->str() + ",battleField=" + battleField->str() + "]";
}


//BAI 4.2
ARVN::ARVN(Unit** unitArray, int size, const string& name, BattleField* battleField)
    : Army(const_cast<Unit**>(unitArray), size, name, battleField) 
  // Tạo Army trước (dummy init), tí set lại unitList
{
    unitList = new UnitList(LF, EXP);

    // Duyệt qua từng đơn vị để thêm vào unitList
    for (int i = 0; i < size; ++i) {
        unitList->insert(const_cast<Unit*>(unitArray[i]));  // Bạn đã làm rồi
        Unit* nonConstUnit = const_cast<Unit*>(unitArray[i]);
        // Bỏ qua const khi gọi getAttackScore()
        if (dynamic_cast<Vehicle*>(nonConstUnit)) {
            LF += nonConstUnit->getAttackScore();
        } else if (dynamic_cast<Infantry*>(nonConstUnit)) {
            EXP += nonConstUnit->getAttackScore();
        }

    }
}

void ARVN::fight(Army* enemy, bool defense) {
    if (!defense) { 
        // Trường hợp ARVN tấn công (defense = false)
        // Quân đội Sài Gòn thất bại, mất 20% quantity mỗi đơn vị
        for (int i = 0; i < unitList->size(); ) {
            Unit* unit = unitList->get(i);
            unit->decreaseQuantity(20);

            if (unit->getQuantity() <= 1) {
                unitList->remove(unit);
            } else {
                ++i;
            }
        }
        // Cập nhật lại LF và EXP
        LF = 0;
        EXP = 0;
        for (int i = 0; i < unitList->size(); ++i) {
            Unit* unit = unitList->get(i);
            if (dynamic_cast<Vehicle*>(unit)) {
                LF += unit->getAttackScore();
            } else if (dynamic_cast<Infantry*>(unit)) {
                EXP += unit->getAttackScore();
            }
        }
    } else {
        // Trường hợp ARVN phòng thủ (defense = true)
        // Nếu thua, bị tịch thu và giảm 20% trọng số
        for (int i = 0; i < unitList->size(); ++i) {
            Unit* unit = unitList->get(i);
            unit->decreaseWeight(20);  // cần có hàm decreaseWeight
        }
        // (Ghi chú thêm: nếu bị enemy lấy mất unit thì xóa đơn vị khỏi unitList.)
    }
}

std::string ARVN::str() const {
    std::string result = "ARVN[attr_name=";
    result += name + "," + std::to_string(LF) + "," + std::to_string(EXP) + ",";
    result += unitList->str() + "," + battleField->str();
    result += "]";
    return result;
}

//BÀI 5
// Thêm đơn vị vào danh sách
bool UnitList::insert(Unit* unit) {
    if (size() >= capacity) {
        return false;  // Không thể thêm nếu vượt quá sức chứa
    }

    // Nếu đơn vị là phương tiện, thêm vào cuối danh sách
    if (Vehicle* vehicle = dynamic_cast<Vehicle*>(unit)) {
        if (vehicleCount == 0) {
            head = new Node(unit);
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = new Node(unit);
        }
        vehicleCount++;
    }
    // Nếu đơn vị là bộ binh, thêm vào đầu danh sách
    else if (Infantry* infantry = dynamic_cast<Infantry*>(unit)) {
        Node* newNode = new Node(unit);
        newNode->next = head;
        head = newNode;
        infantryCount++;
    }
    return true;
}

// Kiểm tra nếu danh sách có chứa một loại phương tiện (VehicleType)
bool UnitList::isContain(VehicleType vehicleType) {
    Node* current = head;
    while (current != nullptr) {
        Vehicle* vehicle = dynamic_cast<Vehicle*>(current->unit);
        if (vehicle && vehicle->getVehicleType() == vehicleType) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Kiểm tra nếu danh sách có chứa một loại bộ binh (InfantryType)
bool UnitList::isContain(InfantryType infantryType) {
    Node* current = head;
    while (current != nullptr) {
        Infantry* infantry = dynamic_cast<Infantry*>(current->unit);
        if (infantry && infantry->getInfantryType() == infantryType) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Trả về chuỗi mô tả danh sách đơn vị
std::string UnitList::str() const {
    std::string result = "UnitList[count_vehicle=" + std::to_string(vehicleCount) +
                         ";count_infantry=" + std::to_string(infantryCount) + ";";
    Node* current = head;
    while (current != nullptr) {
        result += current->unit->str();  // Gọi phương thức str() của mỗi đơn vị
        if (current->next != nullptr) {
            result += ", ";
        }
        current = current->next;
    }
    result += "]";
    return result;
}

// Trả về số lượng đơn vị trong danh sách
int UnitList::size() const {
    int count = 0;
    Node* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

// Lấy đơn vị tại vị trí index
Unit* UnitList::get(int index) const {
    int count = 0;
    Node* current = head;
    while (current != nullptr) {
        if (count == index) {
            return current->unit;
        }
        count++;
        current = current->next;
    }
    return nullptr;  // Trả về nullptr nếu không tìm thấy
}

// Xóa đơn vị khỏi danh sách
void UnitList::remove(Unit* unit) {
    if (head == nullptr) return;

    // Nếu đơn vị cần xóa là đầu danh sách
    if (head->unit == unit) {
        Node* temp = head;
        head = head->next;
        delete temp;
        return;
    }

    Node* current = head;
    while (current->next != nullptr) {
        if (current->next->unit == unit) {
            Node* temp = current->next;
            current->next = current->next->next;
            delete temp;
            return;
        }
        current = current->next;
    }
}

// BÀI 6
// Constructor với hai tham số (mặc định là 0)
Position::Position(int r, int c) : r(r), c(c) {}

// Constructor từ chuỗi str_pos
Position::Position(const string &str_pos) {
    // Loại bỏ dấu ngoặc và tách chuỗi thành các giá trị r và c
    // Giả sử chuỗi luôn ở dạng "(r,c)" như ví dụ "(1,15)"
    if (str_pos.front() == '(' && str_pos.back() == ')') {
        stringstream ss(str_pos.substr(1, str_pos.size() - 2));
        char comma;  // Dùng để đọc dấu phẩy
        ss >> r >> comma >> c;
    }
}

// Getter cho Row
int Position::getRow() const {
    return r;
}

// Getter cho Column
int Position::getCol() const {
    return c;
}

// Setter cho Row
void Position::setRow(int r) {
    this->r = r;
}

// Setter cho Column
void Position::setCol(int c) {
    this->c = c;
}

// Phương thức trả về chuỗi "(r,c)"
string Position::str() const {
    return "(" + to_string(r) + "," + to_string(c) + ")";
}

// BÀI 7
TerrainElement::TerrainElement() {
    // Có thể để trống nếu không có xử lý đặc biệt
}

// Destructor
TerrainElement::~TerrainElement() {
    // Có thể để trống luôn vì thường sẽ có Destructor ảo để đảm bảo đa hình
}

Road::Road(Position pos) {
    this->pos = pos;  // Khởi tạo vị trí của đường
}

Road::~Road() {
    // Xử lý nếu cần thiết
}

void Road::getEffect(Army* army) {
    // Không gây ảnh hưởng đến chỉ số của quân đội
}

void Mountain::getEffect(Army* army) {
    string armyName = army->getName();  
    bool isLiberation = (armyName == "GiaiPhong");

    int radius = isLiberation ? 2 : 4;
    float infantryExpBonus = isLiberation ? 0.3f : 0.2f;
    float vehicleLfPenalty = isLiberation ? 0.1f : 0.05f;

    vector<Unit*> units = army->getUnitList()->getAllUnits();
    Position terrainPos = this->position;

    for (Unit* unit : units) {
        Position unitPos = unit->getCurrentPosition();  // ĐẢM BẢO HÀM getCurrentPosition() HOẠT ĐỘNG

        float dist = sqrt(pow(unitPos.getRow() - terrainPos.getRow(), 2) +
                          pow(unitPos.getCol() - terrainPos.getCol(), 2));

        if (dist <= radius) {
            if (Infantry* inf = dynamic_cast<Infantry*>(unit)) {
                int bonus = static_cast<int>(inf->getAttackScore() * infantryExpBonus);
                inf->setQuantity(inf->getQuantity() + bonus);
            }
            else if (Vehicle* veh = dynamic_cast<Vehicle*>(unit)) {
                int penalty = static_cast<int>(veh->getAttackScore() * vehicleLfPenalty);
                veh->setQuantity(veh->getQuantity() - penalty);
                if (veh->getQuantity() < 0) veh->setQuantity(0);
            }
        }
    }
}

double Position::distanceTo(const Position &other) const {
    int dr = r - other.r;
    int dc = c - other.c;
    return sqrt(dr * dr + dc * dc);
}

void River::getEffect(Army* army) {
    for (int i = 0; i < army->getUnitList()->size(); ++i) {
        Unit* unit = army->getUnitList()->get(i);
        if (Infantry* inf = dynamic_cast<Infantry*>(unit)) {
            if (pos.distanceTo(inf->getCurrentPosition()) <= 2.0) {
                // Tính penalty giảm attackScore cho các đơn vị bộ binh trong bán kính 2 đơn vị
                int penalty = static_cast<int>(inf->getAttackScore() * 0.1);
                // Giảm attackScore của bộ binh
                inf->setAttackScore(inf->getAttackScore() - penalty);
            }
        }
    }
}


void Urban::getEffect(Army* army) {
    for (int i = 0; i < army->getUnitList()->size(); ++i) {
        Unit* unit = army->getUnitList()->get(i);
        Position unitPos = unit->getCurrentPosition();  // Vị trí của đơn vị quân sự

        // Tính khoảng cách giữa đơn vị quân sự và vị trí khu dân cư
        double dist = pos.distanceTo(unitPos);

        // Kiểm tra nếu khoảng cách trong bán kính 5 đơn vị
        if (dist <= 5.0) {
            if (Infantry* inf = dynamic_cast<Infantry*>(unit)) {
                if (inf->getInfantryType() == SPECIALFORCES || inf->getInfantryType() == REGULARINFANTRY) {
                    // Tăng attackScore cho SPECIALFORCES hoặc REGULARINFANTRY trong bán kính 5 đơn vị
                    int bonus = static_cast<int>(2 * inf->getAttackScore() / dist);
                    inf->setAttackScore(inf->getAttackScore() + bonus);
                }
            }
        }

        // Kiểm tra nếu khoảng cách trong bán kính 2 đơn vị đối với ARTILLERY
        if (dist <= 2.0) {
            if (Vehicle* veh = dynamic_cast<Vehicle*>(unit)) {
                if (veh->getVehicleType() == ARTILLERY) {
                    // Giảm attackScore của phương tiện chiến đấu ARTILLERY trong bán kính 2 đơn vị
                    int penalty = static_cast<int>(veh->getAttackScore() * 0.5);
                    veh->setAttackScore(veh->getAttackScore() - penalty);
                }
            }
        }

        // Kiểm tra nếu khoảng cách trong bán kính 3 đơn vị đối với REGULARINFANTRY
        if (dist <= 3.0) {
            if (Infantry* inf = dynamic_cast<Infantry*>(unit)) {
                if (inf->getInfantryType() == REGULARINFANTRY) {
                    // Tăng attackScore cho REGULARINFANTRY trong bán kính 3 đơn vị
                    int bonus = static_cast<int>(3 * inf->getAttackScore() / (2 * dist));
                    inf->setAttackScore(inf->getAttackScore() + bonus);
                }
            }
        }
    }
}


void Fortification::getEffect(Army *army) {
    for (int i = 0; i < army->getUnitList()->size(); ++i) {
        Unit* unit = army->getUnitList()->get(i);
        double distance = pos.distanceTo(unit->getCurrentPosition());

        if (distance <= 2.0) {
            int change = static_cast<int>(unit->getAttackScore() * 0.2);

            if (army->getName() == "Quân Giải Phóng") {
                unit->decreaseAttackScore(change);
            }
            else if (army->getName() == "Quân đội Sài Gòn") {
                unit->increaseAttackScore(change);
            }
        }
    }
}


void SpecialZone::getEffect(Army *army) {
    for (int i = 0; i < army->getUnitList()->size(); ++i) {
        Unit* unit = army->getUnitList()->get(i);
        if (pos.distanceTo(unit->getCurrentPosition()) <= 1.0) {
            // Giảm attackScore của tất cả đơn vị quân sự trong bán kính 1 đơn vị quanh khu vực phi quân sự
            unit->decreaseAttackScore(unit->getAttackScore());  // Giảm hết attackScore về 0
        }
    }
}

// BÀI 8
BattleField::BattleField(int n_rows, int n_cols, vector<Position*> arrayForest,
    vector<Position*> arrayRiver, vector<Position*> arrayFortification,
    vector<Position*> arrayUrban, vector<Position*> arraySpecialZone)
: n_rows(n_rows), n_cols(n_cols)
{
    terrain = new TerrainElement**[n_rows];
    for (int i = 0; i < n_rows; ++i) 
    {
        terrain[i] = new TerrainElement*[n_cols];
        for (int j = 0; j < n_cols; ++j) 
        {
            terrain[i][j] = new Road(Position(i, j)); // Mặc định là đường mòn (Road)
        }
    }

    // Gán các địa hình đặc biệt cho các vị trí tương ứng
    for (Position* pos : arrayForest)
    setTerrainAt(*pos, new Mountain(*pos));
    for (Position* pos : arrayRiver)
    setTerrainAt(*pos, new River(*pos));
    for (Position* pos : arrayFortification)
    setTerrainAt(*pos, new Fortification(*pos));
    for (Position* pos : arrayUrban)
    setTerrainAt(*pos, new Urban(*pos));
    for (Position* pos : arraySpecialZone)
    setTerrainAt(*pos, new SpecialZone(*pos));
}

void BattleField::setTerrainAt(Position& pos, TerrainElement* element) {
    int r = pos.getRow(), c = pos.getCol();
    if (r >= 0 && r < n_rows && c >= 0 && c < n_cols) {
        delete terrain[r][c]; // Xóa phần tử cũ
        terrain[r][c] = element;
    }
}

BattleField::~BattleField() {
    for (int i = 0; i < n_rows; ++i) {
            for (int j = 0; j < n_cols; ++j) {
            delete terrain[i][j]; // Giải phóng bộ nhớ cho mỗi phần tử TerrainElement
        }
        delete[] terrain[i];
    }
    delete[] terrain; // Giải phóng bộ nhớ cho mảng terrain
}

// string BattleField::str() const {
//     return "BattleField[n_rows=" + to_string(n_rows) + ", n_cols=" + to_string(n_cols) + "]";
// }

TerrainElement* BattleField::getTerrainAt(int row, int col) const {
    if (row >= 0 && row < n_rows && col >= 0 && col < n_cols) 
    {
        return terrain[row][col];
    }
    return nullptr;
}

// BÀI 9
Configuration::~Configuration() {
    for (auto& unit : liberationUnits) {
        delete unit;
    }
    for (auto& unit : ARVNUnits) {
        delete unit;
    }
    for (auto& pos : arrayForest) {
        delete pos;
    }
    for (auto& pos : arrayRiver) {
        delete pos;
    }
    for (auto& pos : arrayFortification) {
        delete pos;
    }
    for (auto& pos : arrayUrban) {
        delete pos;
    }
    for (auto& pos : arraySpecialZone) {
        delete pos;
    }
}

void Configuration::parsePositionArray(const string& str, vector<Position*>& array) {
    if (str.empty() || str == "[]") return;
    
    cout << "Debug: Parsing position array: " << str << endl;
    
    // Remove the square brackets and any leading '=' if present
    string cleanedStr = str;
    if (cleanedStr[0] == '=') {
        cleanedStr = cleanedStr.substr(1);
    }
    cleanedStr = cleanedStr.substr(1, cleanedStr.size() - 2);
    
    // Find all positions in the format (r,c)
    size_t start = 0;
    while (start < cleanedStr.size()) {
        size_t openBracket = cleanedStr.find('(', start);
        if (openBracket == string::npos) break;
        
        size_t closeBracket = cleanedStr.find(')', openBracket);
        if (closeBracket == string::npos) break;
        
        string posStr = cleanedStr.substr(openBracket, closeBracket - openBracket + 1);
        cout << "Debug: Processing position string: " << posStr << endl;
        
        // Extract the numbers between parentheses
        string numbers = posStr.substr(1, posStr.size() - 2);
        stringstream numStream(numbers);
        string rStr, cStr;
        getline(numStream, rStr, ',');
        getline(numStream, cStr);
        
        // Convert to integers and create Position object
        int r = stoi(rStr);
        int c = stoi(cStr);
        cout << "Debug: Created position: (" << r << "," << c << ")" << endl;
        array.push_back(new Position(r, c));
        
        start = closeBracket + 1;
    }
}

void Configuration::parseUnitList(const string& str, vector<Unit*>& units, int armyBelong) {
    if (str.empty() || str == "[]") return;
    
    cout << "Debug: Parsing unit list: " << str << endl;
    
    // Remove the square brackets
    string cleanedStr = str.substr(1, str.size() - 2);
    
    // Split the string into individual unit strings
    vector<string> unitStrings;
    string currentUnit;
    int bracketCount = 0;
    
    for (char c : cleanedStr) {
        if (c == '(') bracketCount++;
        else if (c == ')') bracketCount--;
        
        currentUnit += c;
        
        if (c == ')' && bracketCount == 0) {
            unitStrings.push_back(currentUnit);
            currentUnit.clear();
        }
    }
    
    // Process each unit string
    for (const string& unitStr : unitStrings) {
        // Find the type and parameters
        size_t typeEnd = unitStr.find('(');
        if (typeEnd == string::npos) continue;
        
        string unitType = unitStr.substr(0, typeEnd);
        // Remove any leading comma
        if (!unitType.empty() && unitType[0] == ',') {
            unitType = unitType.substr(1);
        }
        string params = unitStr.substr(typeEnd + 1, unitStr.size() - typeEnd - 2);
        
        cout << "Debug: Processing unit - Type: " << unitType << ", Params: " << params << endl;
        
        // Parse parameters
        vector<string> tokens;
        string currentToken;
        int parenCount = 0;
        
        for (char c : params) {
            if (c == '(') parenCount++;
            else if (c == ')') parenCount--;
            
            if (c == ',' && parenCount == 0) {
                tokens.push_back(currentToken);
                currentToken.clear();
            } else {
                currentToken += c;
            }
        }
        if (!currentToken.empty()) {
            tokens.push_back(currentToken);
        }
        
        if (tokens.size() >= 4) {
            // Parse quantity and weight
            int quantity = stoi(tokens[0]);
            int weight = stoi(tokens[1]);
            
            // Parse position
            string posStr = tokens[2];
            posStr = posStr.substr(1, posStr.size() - 2); // Remove parentheses
            stringstream posStream(posStr);
            string rStr, cStr;
            getline(posStream, rStr, ',');
            getline(posStream, cStr);
            int r = stoi(rStr);
            int c = stoi(cStr);
            Position pos(r, c);
            
            // Parse army
            int army = stoi(tokens[3]);
            
            cout << "Debug: Parsed unit - Type: " << unitType 
                 << ", Quantity: " << quantity 
                 << ", Weight: " << weight 
                 << ", Position: (" << r << "," << c << ")"
                 << ", Army: " << army << endl;
            
            // Only add units that belong to the specified army
            if (army == armyBelong) {
                if (unitType == "TANK") {
                    units.push_back(new Vehicle(quantity, weight, pos, TANK));
                    cout << "Debug: Added TANK unit" << endl;
                } else if (unitType == "REGULARINFANTRY") {
                    units.push_back(new Infantry(quantity, weight, pos, REGULARINFANTRY));
                    cout << "Debug: Added REGULARINFANTRY unit" << endl;
                }
            }
        }
    }
}

Configuration::Configuration(const string& filepath) {
    ifstream file(filepath);
    string line;

    while (getline(file, line)) {
        // Remove whitespace from the line
        string cleanedLine;
        for (char c : line) {
            if (!isspace(c)) {
                cleanedLine += c;
            }
        }
        line = cleanedLine;

        if (line.empty()) continue;  // Skip empty lines

        cout << "Debug: Processing line: " << line << endl;

        if (line.find("NUM_ROWS=") == 0) {
            num_rows = stoi(line.substr(9));
        } else if (line.find("NUM_COLS=") == 0) {
            num_cols = stoi(line.substr(9));
        } else if (line.find("ARRAY_FOREST=") == 0) {
            string forestStr = line.substr(13);
            cout << "Debug: Forest string: " << forestStr << endl;
            parsePositionArray(forestStr, arrayForest);
        } else if (line.find("ARRAY_RIVER=") == 0) {
            string riverStr = line.substr(12);
            cout << "Debug: River string: " << riverStr << endl;
            parsePositionArray(riverStr, arrayRiver);
        } else if (line.find("ARRAY_FORTIFICATION=") == 0) {
            string fortStr = line.substr(19);
            cout << "Debug: Fortification string: " << fortStr << endl;
            parsePositionArray(fortStr, arrayFortification);
        } else if (line.find("ARRAY_URBAN=") == 0) {
            string urbanStr = line.substr(12);
            cout << "Debug: Urban string: " << urbanStr << endl;
            parsePositionArray(urbanStr, arrayUrban);
        } else if (line.find("ARRAY_SPECIAL_ZONE=") == 0) {
            string specialStr = line.substr(18);
            cout << "Debug: Special zone string: " << specialStr << endl;
            parsePositionArray(specialStr, arraySpecialZone);
        } else if (line.find("UNIT_LIST=") == 0) {
            string unitListStr = line.substr(10);
            cout << "Debug: Unit list string: " << unitListStr << endl;
            // Parse units for Liberation Army (armyBelong = 0)
            parseUnitList(unitListStr, liberationUnits, 0);
            // Parse units for ARVN Army (armyBelong = 1)
            parseUnitList(unitListStr, ARVNUnits, 1);
        } else if (line.find("EVENT_CODE=") == 0) {
            eventCode = parseEventCode(line.substr(11));
        }
    }
    file.close();
}

int Configuration::parseEventCode(const string& str) {
    int eventCode = stoi(str);
    if (eventCode < 0) eventCode = 0;
    if (eventCode > 99) eventCode %= 100;
    return eventCode;
}


string Configuration::str() const {
    stringstream ss;
    ss << "[num_rows=" << num_rows << ",num_cols=" << num_cols;

    // In các vùng địa hình
    cout << "Debug: arrayForest size = " << arrayForest.size() << endl;
    ss << ",arrayForest=[";
    for (size_t i = 0; i < arrayForest.size(); ++i) {
        ss << "(" << arrayForest[i]->getRow() << "," << arrayForest[i]->getCol() << ")";
        if (i != arrayForest.size() - 1) ss << ",";  // Không thêm dấu phẩy sau phần tử cuối
    }
    ss << "],";

    ss << "arrayRiver=[";
    for (size_t i = 0; i < arrayRiver.size(); ++i) {
        ss << "(" << arrayRiver[i]->getRow() << "," << arrayRiver[i]->getCol() << ")";
        if (i != arrayRiver.size() - 1) ss << ",";  // Không thêm dấu phẩy sau phần tử cuối
    }
    ss << "],";

    ss << "arrayFortification=[";
    for (size_t i = 0; i < arrayFortification.size(); ++i) {
        ss << "(" << arrayFortification[i]->getRow() << "," << arrayFortification[i]->getCol() << ")";
        if (i != arrayFortification.size() - 1) ss << ",";
    }
    ss << "],";

    ss << "arrayUrban=[";
    for (size_t i = 0; i < arrayUrban.size(); ++i) {
        ss << "(" << arrayUrban[i]->getRow() << "," << arrayUrban[i]->getCol() << ")";
        if (i != arrayUrban.size() - 1) ss << ",";
    }
    ss << "],";

    ss << "arraySpecialZone=[";
    for (size_t i = 0; i < arraySpecialZone.size(); ++i) {
        ss << "(" << arraySpecialZone[i]->getRow() << "," << arraySpecialZone[i]->getCol() << ")";
        if (i != arraySpecialZone.size() - 1) ss << ",";
    }
    ss << "],";

    // In danh sách các đơn vị quân sự
    ss << "liberationUnits=[";
    for (size_t i = 0; i < liberationUnits.size(); ++i) {
        ss << liberationUnits[i]->str();
        if (i != liberationUnits.size() - 1) ss << ",";
    }
    ss << "],";

    ss << "ARVNUnits=[";
    for (size_t i = 0; i < ARVNUnits.size(); ++i) {
        ss << ARVNUnits[i]->str();
        if (i != ARVNUnits.size() - 1) ss << ",";
    }
    ss << "],";

    ss << "eventCode=" << eventCode;

    ss << "]"; // Đóng Configuration

    return ss.str();
}



// BÀI 10
HCMCampaign::HCMCampaign(const string& config_file_path) {
    // Khởi tạo Configuration từ file cấu hình
    config = new Configuration(config_file_path);

    // Khởi tạo BattleField từ các thông tin trong Configuration
    battleField = new BattleField(config->getNumRows(), config->getNumCols(),
                                  config->getForestPositions(), config->getRiverPositions(),
                                  config->getFortificationPositions(), config->getUrbanPositions(),
                                  config->getSpecialZonePositions());

    // Khởi tạo LiberationArmy và ARVN từ danh sách đơn vị trong Configuration
    // Giả sử config->getLiberationUnits() trả về vector<Unit*>
    vector<Unit*> unitArrayOfLiber = config->getLiberationUnits();

    // Chuyển đổi vector<Unit*> thành const Unit**
    Unit** unitArray = new Unit*[unitArrayOfLiber.size()];

    for (size_t i = 0; i < unitArrayOfLiber.size(); ++i) {
        unitArray[i] = unitArrayOfLiber[i];  // Gán các con trỏ từ vector vào mảng con trỏ
    }

    // Khởi tạo LiberationArmy với mảng const Unit**
    LiberationArmy* liberationArmy = new LiberationArmy(unitArray, unitArrayOfLiber.size(), "LiberationArmy", battleField);
}

void Army::prepareForBattle() {
    for (Unit* unit : unitList->getAllUnits()) {
        Position pos = unit->getCurrentPosition();
        TerrainElement* terrain = battleField->getTerrainAt(pos.getRow(), pos.getCol());
        if (terrain) {
            terrain->getEffect(this);  // Giả sử affect(Unit*) đã có
        }
    }

    // Sau khi ảnh hưởng địa hình, cập nhật lại chỉ số LF và EXP
    LF = EXP = 0;
    for (Unit* unit : unitList->getAllUnits()) {
        if (dynamic_cast<Vehicle*>(unit)) {
            LF += unit->getAttackScore();
        } else if (dynamic_cast<Infantry*>(unit)) {
            EXP += unit->getAttackScore();
        }
    }
}


void HCMCampaign::run() {
    int eventCode = config->getEventCode();

    // Địa hình ảnh hưởng tới chỉ số
    liberationArmy->prepareForBattle();
    arvnArmy->prepareForBattle();

    if (eventCode < 75) {
        liberationArmy->fight(arvnArmy, false);
        arvnArmy->fight(liberationArmy, true);  // Nếu có xử lý phòng thủ
    } else {
        arvnArmy->fight(liberationArmy, false);
        liberationArmy->fight(arvnArmy, false);
    }

    // Sau mỗi giao tranh, loại bỏ đơn vị yếu
    liberationArmy->removeWeakUnits();
    arvnArmy->removeWeakUnits();
}


string HCMCampaign::printResult() {
    return "LIBERATIONARMY[LF=" + to_string(liberationArmy->getLF()) + 
           ",EXP=" + to_string(liberationArmy->getEXP()) + "]-" +
           "ARVN[LF=" + to_string(arvnArmy->getLF()) + 
           ",EXP=" + to_string(arvnArmy->getEXP()) + "]";
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////