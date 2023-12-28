#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#include "Store.h"
#include "account.h"

// 用户交互式菜单
void menuforadmin();
void menuforuser();


int main() {
    UserManager userManager("users.txt");
    userManager.loadUsers();

    std::string enteredUsername, enteredPassword;
    std::cout << "请输入用户名: ";
    std::cin >> enteredUsername;
    std::cout << "请输入密码: ";
    std::cin >> enteredPassword;

    User* currentUser = nullptr;

    for (const auto& user : userManager.getUsers()) 
    {
        if (user.login(enteredPassword)) {
            currentUser = new User(user);
            break;
        }
    }
    if (currentUser == nullptr){
        std::cout << "登录失败，密码或账号错误!\n";
        system("pause");
    }
    else if (currentUser->hasAdminPermissions() == true) {
        std::cout << "管理员登录成功！\n";

        Store store;
        std::string name, category;
        std::vector<int> null_number;
        int number, quantity, select;

        // 检查产品信息文件是否存在
        std::ifstream productFile("products.txt");
        if (!productFile.is_open()) 
        {
            // 如果文件不存在，创建示例产品并保存到文件
            std::cout << "未找到产品信息文件，将创建示例产品并保存。\n";

            // 添加示例产品
            Product product1{"ProductA", "家具", 1, std::time(nullptr), 100};
            Product product2{"ProductB", "玩具", 2, std::time(nullptr), 150};

            store.addProduct(product1);
            store.addProduct(product2);

            // 保存产品信息到文件
            store.saveProductsToFile("products.txt");
        }
        else
        {
            store.loadProductsFromFile("products.txt");
        }
        do
        {
            menuforadmin();
            std::cin >> select;
            switch (select)
            {
                case 1: //显示所有产品
                    std::cout << "现有产品信息：\n";
                    store.displayAllProductInfo();
                    continue;
                case 2: // 查找特定产品
                    int i;
                    std::cout << "输入你想要查找的产品编号:";
                    std::cin >> i;
                    store.displayProductInfo(i);
                    continue;
                case 3: // 添加一个产品
                    std::cout << "请输入你想要添加的产品名称:" << std::endl;
                    std::cin >> name;
                    std::cout << "请输入你想要添加的产品类别:" << std::endl;
                    std::cin >> category; 
                    std::cout << "当前有以下编号未被使用:";
                    null_number = store.findMissingProductIds();
                    for(auto i : null_number)
                        std::cout << i << ' ';
                    std::cout << "\n您还可以在该编号后有序的增加序号：" << store.max_number() << std::endl;
                    std::cout << "输入编号:";
                    std::cin >> number;
                    std::cout << "输入产品数量:";
                    std::cin >> quantity;
                    store.addProduct(Product {name, category, number, std::time(nullptr), quantity});
                    store.saveProductsToFile("products.txt");
                    store.loadProductsFromFile("products.txt");
                    continue;
                case 4: // 添加变动记录 
                    std::cout << "请输入要添加的货物变动记录的产品编号，以及它的变动数量(大于0为进货，小于0为销售):";
                    std::cin >> number >> quantity;
                    store.addSale(number, quantity);
                    continue;
                case 5: // 删除指定产品
                    std::cout << "请输入将要删除的产品编号";
                    std::cin >> number;
                    store.deleteProduct(number);
                    store.saveProductsToFile("products.txt");
                    store.loadProductsFromFile("products.txt");
                    continue;
                case 6: // 查询某一产品类别的所有产品
                    std::cout << "当前存在的所有类别为: ";
                    store.displayCategoryList();
                    std::cout << "请输入你想要查找的类别： ";
                    std::cin >> category;
                    store.displayAimCategoryProduct(category);
                    continue;
            }
        }while (select != 0);

        // 保存产品信息到文件
        store.saveProductsToFile("products.txt");

        // 释放用户对象
        delete currentUser;

        system("pause");
    } 
    else if(currentUser->hasAdminPermissions() == false)
    {
        std::cout << "用户登录成功!";
        Store store;
        int select;

        // 检查产品信息文件是否存在
        std::ifstream productFile("products.txt");
        if (!productFile.is_open()) 
        {
            // 如果文件不存在，提示并退出
            std::cout << "尚未存在可访问的数据！请等待管理员创建数据\n";
            system("pause");
        }
        else
        {
            store.loadProductsFromFile("products.txt");
        }
        do
        {
            menuforuser();
            std::cin >> select;
            switch(select)
            {
                case 1: //显示所有产品
                    std::cout << "现有产品信息：\n";
                    store.displayAllProductInfo();
                    continue;
                case 2: // 查找特定产品
                    int i;
                    std::cout << "输入你想要查找的产品编号:";
                    std::cin >> i;
                    store.displayProductInfo(i);
                    continue;
            }
        }while(select != 0);
    }

    return 0;
}

void menuforadmin() 
{
    using std::cout, std::endl;
    cout << "请在下方的选项中选择你想要的操作: (0键退出)" << endl;
    cout.width(49);
    cout.fill('-');
    cout << '-' << endl;
    cout.width(25);
    cout.fill(' ');
    cout.setf(std::ios_base::left , std::ios_base::adjustfield);
    cout << "1. 显示所有产品信息";
    cout.width(25);
    cout << "2. 显示特定编号的产品信息" << endl;
    cout.width(25);
    cout << "3. 添加新产品";
    cout.width(25);
    cout << "4. 产品数量变动" << endl;
    cout.width(25);
    cout << "5. 删除指定产品";
    cout.width(25);
    cout << "6. 查询某一产品类别的所有产品" << endl;

    return ;
}

void menuforuser()
{
    using std::cout, std::endl;
    cout << "请在下方的选项中选择你想要的操作：(0键退出)" << endl;
    cout.width(49);
    cout.fill('-');
    cout << '-' << endl;
    cout.width(25);
    cout.fill(' ');
    cout.setf(std::ios_base::left, std::ios_base::adjustfield);
    cout << "1. 显示所有产品信息";
    cout.width(25);
    cout << "2. 显示特定编号的产品信息" << endl;

    return;
}