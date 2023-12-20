#pragma once
#include <iostream>
#include <fstream>
#include <vector>

class User {
public:
    User(std::string username, std::string password, bool isAdmin) : username(username), password(password), isAdmin(isAdmin) {}

    bool login(const std::string& enteredPassword) const 
    {
        return enteredPassword == password;
    }

    bool hasAdminPermissions() const 
    {
        return isAdmin;
    }

    static void initializeUsers(const std::string & filename) 
    {
        std::cout << "正在创建初始用户...\n";

        // 创建初始用户
        User adminUser("admin", "adminpassword", true);
        User regularUser("user", "userpassword", false);

        // 将初始用户保存到文件
        std::ofstream file(filename);
        if (file.is_open()) {
            file << adminUser.username << ' ' << adminUser.password << ' ' << adminUser.hasAdminPermissions() << '\n';
            file << regularUser.username << ' ' << regularUser.password << ' ' << regularUser.hasAdminPermissions() << '\n';
            std::cout << "初始用户创建成功。\n";
        } else {
            std::cerr << "无法创建初始用户文件: " << filename << "\n";
        }
    }
private:
    std::string username;
    std::string password;
    bool isAdmin;
};

class UserManager
{
public:
    UserManager(const std::string& filename) : filename(filename) {}

    void loadUsers() {
        std::ifstream file(filename);
        if (file.is_open()) 
        {
            while (file >> std::ws && !file.eof()) {
                std::string username, password;
                bool isAdmin;
                file >> username >> password >> isAdmin;
                users.emplace_back(username, password, isAdmin);
            }
        } 
        else 
        {
            std::cerr << "无法打开文件: " << filename << "\n";

            // 如果文件不存在，创建初始用户并保存到文件
            User::initializeUsers(filename);
        }
    }

    const std::vector<User>& getUsers() const {
        return users;
    }

private:
    std::string filename;
    std::vector<User> users;
};
