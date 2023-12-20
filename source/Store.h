#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <set>

// 结构定义：销售记录
struct SalesRecord {
    int soldQuantity;
    std::time_t saleTime;
};

// 结构定义：产品
struct Product {
    std::string name;
    int id;
    std::time_t purchaseDate;
    int quantity;
    std::vector<SalesRecord> salesRecords;

    // 左右子节点指针
    Product* left;
    Product* right;

    // 构造函数，用于创建产品对象
    Product(const std::string& productName = "defualt name", int productId = 0, std::time_t purchaseDt = std::time_t(nullptr), int productQty = 0)
        : name(productName), id(productId), purchaseDate(purchaseDt), quantity(productQty), left(nullptr), right(nullptr) {}
    
    ~Product() {
    salesRecords.clear();  // 清空销售记录
}
};

// 类定义：库房
class Store {
public:
    // 构造函数，初始化最大产品ID为0
    Store() : root(nullptr), maxProductId(0) {}
    // 向库房中添加产品
    void addProduct(const Product& product);
    // 查询未被使用的编号
    std::vector<int> findMissingProductIds() const;
    // 查询最大编号
    int max_number(){return maxProductId;}
    // 添加销售记录
    void addSale(int productId, int quantity);
    // 删除指定编号的产品
    void deleteProduct(int productId);
    Product* deleteProductHelper(Product* node, int productId);
    Product* findMin(Product* node);
    // 显示产品信息
    void displayProductInfo(int productId) const;
    // 显示所有产品信息
    void displayAllProductInfo() const{
        for(int i = 1; i <= maxProductId; i++)
            displayProductInfo(i);
    }
    // 将产品信息保存到文件
    void saveProductsToFile(const std::string& filename) const;
    // 从文件中加载产品信息
    void loadProductsFromFile(const std::string& filename);

private:
    Product* root;  // 根节点指针
    int maxProductId;  // 最大产品ID

    // 插入产品节点
    Product* insert(Product* node, const Product& product);
    // 搜索产品节点
    Product* search(Product* node, int productId) const;
    // 将产品信息保存到文件
    void saveToFile(Product* node, std::ofstream& file) const;
};