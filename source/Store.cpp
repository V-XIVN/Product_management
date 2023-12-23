#include "Store.h"
static std::vector<int> aim_list;

void Store::addProduct(const Product& product)
{
    // 更新最大产品ID
    maxProductId = std::max(maxProductId, product.id);
    root = insert(root, product);
    update_categorylist(root);
}

void Store::addSale(int productId, int quantity) 
{
    Product* product = search(root, productId);
    if (product) {
        SalesRecord record{quantity, std::time(nullptr)};
        product->salesRecords.push_back(record);
        product->quantity += quantity;
    } else {
        std::cerr << "未找到产品编号为 " << productId << " 的产品。\n";
    }
}

void Store::displayProductInfo(int productId) const 
{
    Product* product = search(root, productId);
    if (product) {
        std::cout << "产品名称: " << product->name << "\n";
        std::cout << "产品类别: " << product->category << "\n";
        std::cout << "产品编号: " << product->id << "\n";
        std::cout << "总数量: " << product->quantity << "\n";
        std::cout << "进货日期: " << std::ctime(&product->purchaseDate);
        std::cout << "销售记录:\n";
        for (const auto& record : product->salesRecords) 
        {
            if(record.soldQuantity < 0)
                std::cout << "售出数量: " << -record.soldQuantity << ", 销售时间: " << std::ctime(&record.saleTime);
            else if(record.soldQuantity > 0)
                std::cout << "进货数量: " << record.soldQuantity << ", 进货时间: " << std::ctime(&record.saleTime);
        }
        std::cout << "----------------------------------" << std::endl;
    } else {
        std::cerr << "未找到产品编号为 " << productId << " 的产品。\n";
        std::cout << "----------------------------------" << std::endl;
    }
}

void Store::saveProductsToFile(const std::string& filename) const 
{
    std::ofstream file(filename);
    if (file.is_open()) {
        saveToFile(root, file);
    } else {
        std::cerr << "无法打开文件: " << filename << "\n";
    }
}

void Store::loadProductsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        Product product;
        /*
        while (file >> product.name >> product.id >> product.purchaseDate >> product.quantity) {
            addProduct(product);
        }
        */
        // 测试加入产品销售记录之后的读取与保存
        int size, salequatity;
        std::time_t sale_time;
        SalesRecord sales;

        while (file >> product.name >> product.category >> product.id >> product.purchaseDate >> product.quantity)
        {
            file >> size;
            if(size != 0)
                for(int j = 0; j < size; j++)
                {
                    file >> salequatity >> sale_time;
                    sales.soldQuantity = salequatity;
                    sales.saleTime = sale_time;
                    product.salesRecords.push_back(sales);
                }
            addProduct(product);
        }
        update_categorylist(root);
    } else {
        std::cerr << "无法打开文件: " << filename << "\n";
    }
}

Product* Store::insert(Product* node, const Product& product) 
{
    if (node == nullptr)
    {
        // 如果编号已存在，则拒绝插入
        if (search(root, product.id))
        {
            std::cerr << "产品编号 " << product.id << " 已存在，插入失败。\n";
            return nullptr;
        }

        return new Product(product);
    }

    if (product.id < node->id)
    {
        node->left = insert(node->left, product);
    }
    else if (product.id > node->id)
    {
        node->right = insert(node->right, product);
    }

    return node;
}

Product* Store::search(Product* node, int productId) const 
{
    if (node == nullptr || node->id == productId) {
        return node;
    }

    if (productId < node->id) {
            return search(node->left, productId);
    } 
    else {
            return search(node->right, productId);
    }
}

void Store::saveToFile(Product* node, std::ofstream& file) const 
{
    if (node != nullptr) {
        saveToFile(node->left, file);
        // file << node->name << ' ' << node->id << ' ' << node->purchaseDate << ' ' << node->quantity << '\n';
        // 测试写入销售数据，销售数据格式跟在产品数据后，先写入销售记录的总数，然后写入销售记录的数量和时间
        file << node->name << ' ' << node->category << ' ' << node->id << ' ' << node->purchaseDate << ' ' << node->quantity << ' ' << node->salesRecords.size();
        for(SalesRecord i : node->salesRecords)
            file << ' ' << i.soldQuantity << ' ' << i.saleTime;
        file << '\n';
        saveToFile(node->right, file);
    }
 }

void findMissingProduct(Product* node, std::set<int>& usedIds, std::vector<int>& missingIds)
{
    if (node != nullptr)
    {
        findMissingProduct(node->left, usedIds, missingIds);

        usedIds.insert(node->id);

        findMissingProduct(node->right, usedIds, missingIds);
    }
}

std::vector<int> Store::findMissingProductIds() const
{
    std::set<int> usedIds;
    std::vector<int> missingIds;


    findMissingProduct(root, usedIds, missingIds);

    int prevId = 0;
    for (int id : usedIds)
    {
        if (id - prevId > 1)
        {
            for (int missingId = prevId + 1; missingId < id; ++missingId)
            {
                missingIds.push_back(missingId);
            }
        }
        prevId = id;
    }

    return missingIds;
}

void Store::deleteProduct(int productId)
{
    root = deleteProductHelper(root, productId);
    update_categorylist(root);
}

Product* Store::deleteProductHelper(Product* node, int productId)
{
    if (node == nullptr)
    {
        std::cerr << "未找到产品编号为 " << productId << " 的产品。\n";
    }
    else if (productId < node->id)
    {
        node->left = deleteProductHelper(node->left, productId);
    }
    else if (productId > node->id)
    {
        node->right = deleteProductHelper(node->right, productId);
    }
    else
    {
        // 找到了要删除的产品
        Product* temp = node;

        // 如果节点没有左子树，右子树替代当前节点
        if (node->left == nullptr)
        {
            node = node->right;
        }
        // 如果节点没有右子树，左子树替代当前节点
        else if (node->right == nullptr)
        {
            node = node->left;
        }
        else
        {
            // 有两个子节点的情况，找到右子树的最小节点
            Product* minNode = findMin(node->right);

            // 将右子树的最小节点的值复制到当前节点
            node->name = minNode->name;
            node->id = minNode->id;
            node->purchaseDate = minNode->purchaseDate;
            node->quantity = minNode->quantity;
            node->salesRecords = minNode->salesRecords;

            // 递归删除右子树的最小节点
            node->right = deleteProductHelper(node->right, minNode->id);
        }

        delete temp;  // 释放当前节点的内存
    }

    return node;
}

Product* Store::findMin(Product* node)
{
    while (node->left != nullptr)
    {
        node = node->left;
    }
    return node;
}

void Store::update_categorylist(Product * node)
{ 
    if(node == root)
        category_list.clear();
    bool is_exist = true;
    for(std::string i : category_list)
    {
        if (i == node->category)
            is_exist = false;
    }
    if(is_exist)
        category_list.push_back(node->category);
    

    if (node->left) {
            return update_categorylist(node->left);
    } 
    else if(node->right) 
    {
            return update_categorylist(node->right);
    }
    else
    {
        return;
    }
}

void Store::displayCategoryList() const
{
    for(std::string i : category_list)
        std::cout << i << ' ';
    std::cout << std::endl; 
    return;
}

void Store::find_categorylist(Product * node, std::string aim)
{
    if (node == root)
        aim_list.clear();
    if(node->category == aim)
        aim_list.push_back(node->id);
    

    if (node->left) {
        return find_categorylist(node->left, aim);
    } 
    else if(node->right) 
    {
        return find_categorylist(node->right, aim);
    }
    else
    {
        return;
    }
}

void Store::displayAimCategoryProduct(std::string aim)
{
    find_categorylist(root, aim);
    for(auto i : aim_list)
    {
        displayProductInfo(i);
    }

}