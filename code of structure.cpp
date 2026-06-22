#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
using namespace std;

#define MAX_MOD 1000    // 平台最多存储1000个MOD
#define DATA_FILE "mod_data.txt"

// MOD结构体：对应网站数据库字段
struct ModSkin {
    int id;                 // MOD唯一编号
    string modName;         // 枪皮名称
    string animeIP;         // 所属二次元IP（初音、原神、东方等）
    string weaponType;      // 武器分类：步枪/手枪/霰弹/近战
    string author;          // 创作者昵称
    int hotValue;           // 热度（浏览+订阅量）
    double fileSize;        // MOD压缩包大小MB
    bool isAudit;           // true=审核通过，false=待审核
};

ModSkin modList[MAX_MOD];
int modCount = 0;          // 当前MOD总数

// 函数声明
void loadData();                    // 从文件读取历史MOD数据
void saveData();                    // 保存MOD数据到文件
bool checkUploadSuffix(string name);// 上传文件后缀校验算法
void addMod();                      // MOD上传录入（模拟网站上传）
void showAllMod();                  // 全部资源展示
int binarySearchById(int targetId); // 二分查找：按ID查找MOD
void searchModByKeyword();          // 关键词模糊查找
void sortByHot();                   // 冒泡排序：按热度降序排行
void auditMod();                    // 管理员审核功能
void menu();                        // 主菜单

int main() {
    loadData();
    int op;
    while (true) {
        menu();
        cout << "请输入功能序号：";
        cin >> op;
        cin.ignore();
        switch(op) {
            case 1: addMod(); break;
            case 2: showAllMod(); break;
            case 3: searchModByKeyword(); break;
            case 4: sortByHot(); break;
            case 5: auditMod(); break;
            case 0:
                saveData();
                cout << "数据已保存，程序退出\n";
                system("pause");
                return 0;
            default: cout << "输入错误，请重新选择\n";
        }
        system("pause");
        system("cls");
    }
    return 0;
}

// 读取本地文件模拟数据库
void loadData() {
    ifstream fin(DATA_FILE);
    if (!fin.is_open()) {
        cout << "未找到历史数据文件，初始化空白平台\n";
        modCount = 0;
        return;
    }
    fin >> modCount;
    fin.ignore();
    for (int i = 0; i < modCount; i++) {
        fin >> modList[i].id;
        fin.ignore();
        getline(fin, modList[i].modName);
        getline(fin, modList[i].animeIP);
        getline(fin, modList[i].weaponType);
        getline(fin, modList[i].author);
        fin >> modList[i].hotValue >> modList[i].fileSize >> modList[i].isAudit;
        fin.ignore();
    }
    fin.close();
    cout << "成功载入" << modCount << "条MOD资源数据\n";
}

// 保存数据到文件
void saveData() {
    ofstream fout(DATA_FILE);
    fout << modCount << endl;
    for (int i = 0; i < modCount; i++) {
        fout << modList[i].id << endl;
        fout << modList[i].modName << endl;
        fout << modList[i].animeIP << endl;
        fout << modList[i].weaponType << endl;
        fout << modList[i].author << endl;
        fout << modList[i].hotValue << " " << modList[i].fileSize << " " << modList[i].isAudit << endl;
    }
    fout.close();
}

// 上传后缀校验算法：只允许zip/rar MOD压缩包，拦截exe等恶意文件
bool checkUploadSuffix(string name) {
    size_t pos = name.find_last_of(".");
    if (pos == string::npos) return false;
    string suffix = name.substr(pos);
    if (suffix == ".zip" || suffix == ".rar")
        return true;
    return false;
}

// 1. MOD上传提交功能
void addMod() {
    if (modCount >= MAX_MOD) {
        cout << "平台资源已满，无法继续上传\n";
        return;
    }
    ModSkin newMod;
    newMod.id = modCount + 1;
    cout << "=====MOD作品上传提交=====\n";
    cout << "输入枪皮作品名称：";
    getline(cin, newMod.modName);
    cout << "所属二次元IP(如初音/原神/东方)：";
    getline(cin, newMod.animeIP);
    cout << "武器分类(步枪/手枪/霰弹/近战)：";
    getline(cin, newMod.weaponType);
    cout << "创作者昵称：";
    getline(cin, newMod.author);
    cout << "MOD压缩包文件名：";
    string fileName;
    getline(cin, fileName);

    // 调用上传校验算法
    if (!checkUploadSuffix(fileName)) {
        cout << "上传失败！仅支持 .zip .rar MOD资源包\n";
        return;
    }
    cout << "文件大小(MB)：";
    cin >> newMod.fileSize;
    newMod.hotValue = 0;
    newMod.isAudit = false; // 新上传默认待审核

    modList[modCount] = newMod;
    modCount++;
    saveData();
    cout << "MOD上传成功，等待管理员审核，作品ID：" << newMod.id << endl;
}

// 2. 展示全部MOD资源（对应网站首页列表）
void showAllMod() {
    cout << left << setw(6) << "ID"
         << setw(18) << "作品名"
         << setw(12) << "二次元IP"
         << setw(10) << "武器分类"
         << setw(10) << "作者"
         << setw(8) << "热度"
         << setw(10) << "大小(MB)"
         << setw(8) << "审核状态" << endl;
    cout << "------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < modCount; i++) {
        string auditStr = modList[i].isAudit ? "已通过" : "待审核";
        cout << left << setw(6) << modList[i].id
             << setw(18) << modList[i].modName
             << setw(12) << modList[i].animeIP
             << setw(10) << modList[i].weaponType
             << setw(10) << modList[i].author
             << setw(8) << modList[i].hotValue
             << setw(10) << modList[i].fileSize
             << setw(8) << auditStr << endl;
    }
}

// 二分查找：根据MOD编号精准查找
int binarySearchById(int targetId) {
    int l = 0, r = modCount - 1;
    while (l <= r) {
        int mid = (l + r) / 2;
        if (modList[mid].id == targetId)
            return mid;
        else if (modList[mid].id < targetId)
            l = mid + 1;
        else
            r = mid - 1;
    }
    return -1;
}

// 3. 关键词检索（搜索功能：匹配名称/IP/武器分类）
void searchModByKeyword() {
    cout << "输入搜索关键词：";
    string key;
    getline(cin, key);
    bool findFlag = false;
    cout << "\n【搜索结果】\n";
    for (int i = 0; i < modCount; i++) {
        if (modList[i].modName.find(key) != string::npos
            || modList[i].animeIP.find(key) != string::npos
            || modList[i].weaponType.find(key) != string::npos) {
            string auditStr = modList[i].isAudit ? "已通过" : "待审核";
            cout << "ID:" << modList[i].id
                 << " 名称:" << modList[i].modName
                 << " IP:" << modList[i].animeIP
                 << " 分类:" << modList[i].weaponType
                 << " 审核:" << auditStr << endl;
            findFlag = true;
        }
    }
    if (!findFlag) cout << "未匹配到相关MOD资源\n";
}

// 4. 冒泡排序：按热度降序（网站热门排行榜算法）
void sortByHot() {
    for (int i = 0; i < modCount - 1; i++) {
        for (int j = 0; j < modCount - 1 - i; j++) {
            if (modList[j].hotValue < modList[j+1].hotValue) {
                ModSkin temp = modList[j];
                modList[j] = modList[j+1];
                modList[j+1] = temp;
            }
        }
    }
    cout << "已按热度从高到低排序，排序结果：\n";
    showAllMod();
    saveData();
}

// 5. 管理员审核模块
void auditMod() {
    int aid;
    cout << "输入需要审核的MOD ID：";
    cin >> aid;
    int pos = binarySearchById(aid);
    if (pos == -1) {
        cout << "不存在该ID作品\n";
        return;
    }
    if (modList[pos].isAudit) {
        cout << "该作品已经审核通过\n";
        return;
    }
    int op;
    cout << "1-审核通过  2-驳回下架  请选择：";
    cin >> op;
    if (op == 1) {
        modList[pos].isAudit = true;
        cout << "审核完成，作品公开展示\n";
    } else {
        // 驳回删除该MOD
        for (int i = pos; i < modCount - 1; i++) {
            modList[i] = modList[i+1];
        }
        modCount--;
        cout << "作品已驳回移除\n";
    }
    saveData();
}

// 主菜单界面（网站导航模拟）
void menu() {
    cout << "\n======== Steam二次元枪皮MOD资源平台（后台框架） ========\n";
    cout << "1. MOD创作者上传提交作品\n";
    cout << "2. 浏览全部MOD资源列表\n";
    cout << "3. 关键词搜索MOD资源\n";
    cout << "4. 按热度排序（热门榜单）\n";
    cout << "5. 管理员作品审核\n";
    cout << "0. 保存数据并退出系统\n";
    cout << "========================================================\n";
}
