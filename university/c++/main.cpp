#include <iomanip>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>

#include "graph_node.h"
#include "dijkstra_algo.h"
struct Point
{
    double x, y, z;
};

std::ostream& operator << (std::ostream& out, Point p)
{
    std::cout << '(' << p.x << ',' << p.y << ',' << p.z << ')';
    return out;
}

int main()
{
    /*
    Graph<std::string, Point, double> graph;
    graph["zero"]; // Заполнится точкой, которая заполнится нулями
    auto [it1, flag1] = graph.insert_node("first", {1, 1, 1});
    std::cout << std::boolalpha << flag1 << std::endl; // => true
    
    graph["second"]; // Заполнится точкой, которая заполнится нулями
    auto [it2, flag2] = graph.insert_or_assign_node("second", {2, 2, 2}); // перезаполнит
    std::cout << std::boolalpha << flag2 << std::endl; // => false
    
    graph["third"] = Point{ 3, 3, 3 };
    auto [it3, flag3] = graph.insert_node("third", {1, 1, 1}); // бездействует
    std::cout << std::boolalpha << flag3 << std::endl; // => false
    
    graph["fourth"]; // Заполнится точкой, которая заполнится нулями
    graph.at("fourth") = Point{ 4, 4, 4 };
    
    try
    {
        graph.at("fifth");
    }
    catch (const char* msg)
    {
        std::cout << msg << std::endl;
        
    }
    
    auto [it4, flag4] = graph.insert_edge({ "first", "second" }, 44.44);
    std::cout << std::boolalpha << flag4 << std::endl; // => true
    
    auto [it5, flag5] = graph.insert_edge({ "first", "second" }, 55.55);
    std::cout << std::boolalpha << flag5 << std::endl; // => false
    
    auto [it6, flag6] = graph.insert_or_assign_edge({ "first", "second" }, 66.66);
    std::cout << std::boolalpha << flag6 << std::endl; // => false
    
    auto [it7, flag7] = graph.insert_or_assign_edge({ "second", "first" }, 77.77);
    std::cout << std::boolalpha << flag7 << std::endl; // => true
    
    
    auto graph_other = graph; // Конструктор копирования
    auto graph_new = std::move(graph); // Конструктор перемещения graph = std::move(graph_new); // Перемещающее присваивание
    
    
    graph_new = graph; // Копирующее присваивание
    graph.swap(graph_new); // Поменять местами содержимое графов
    swap(graph, graph_new); // Поменять местами содержимое графов
     
    
    for (auto& [key, node] : graph)
    {
        cout << "Is here no edges?" << std::boolalpha << node.empty() << std::endl;
        cout << "How many edges are going from here?" << node.size() << std::endl;
        node.value() = Point{ 1,2,3 }; // могу поменять вес
        for (auto& [key, weight] : node)
        {
            //key = "new key"; // ОШИБКА: нельзя менять ключ
            weight = 11.11; // могу задать новый вес у этого ребра
        }
        
        bool flag = node.erase_edge("first");
    }
    
    bool flag8 = graph.erase_node("new name");
    std::cout << std::boolalpha << flag8 << std::endl;
    bool flag9 = graph.erase_node("first");
    std::cout << std::boolalpha << flag9 << std::endl;
    
    try
    {
        graph.insert_edge({ "second", "zero" }, 4.4);
        
    }
    catch (const char* msg)
    {
        std::cout << msg << std::endl;
        
    }
    
    try
    {
        graph.insert_edge({ "third", "second" }, 6.6);
        
    }
    catch (const char* msg)
    {
        std::cout << msg << std::endl;
        
    }
    */
    /*
    Graph<string, int, int> g;
    auto [flag1, flag2] = g.insert_node("one", 5);
    cout << flag1->second.value()<<" "<< flag2 << endl;
    auto [flag_1, flag_2] = g.insert_or_assign_node("one", 6);
    cout << flag_1->second.value() <<" "<< flag_2 << endl;
    g["one"] = 7;
    cout << g["one"] << endl;
    g["three"];
    g["two"] = 90;
    cout << g["three"] << endl;
    auto [flag11, flag22] = g.insert_edge({"one", "three"}, 5);
    cout << flag11->second.weight_t("three") << endl;
    auto [flag_11, flag_22] = g.insert_or_assign_edge({"one", "three"}, 9);
    cout << flag_11->second.weight_t("three") << endl;
    auto [flag111, flag222] = g.insert_edge({"one", "three"}, 5);
    cout << flag111->second.weight_t("three") << endl;
    */
    
    Graph<int, double, double> graph;

    graph.insert_node(0, 0);
    graph.insert_node(1, 0);
    graph.insert_node(2, 0);
    graph.insert_node(3, 0);
    graph.insert_node(4, 0);

    graph.insert_edge({0, 0}, 5);
    graph.insert_edge({1, 3}, 2);
    graph.insert_edge({2, 0}, 1.2);
    graph.insert_edge({2, 3}, 15);
    graph.insert_edge({2, 4}, 10.5);
    graph.insert_edge({3, 1}, 3);
    graph.insert_edge({4, 3}, 1);
    graph.insert_edge({4, 1}, 8);
    graph[5] = 123;

    
    try
    {
        auto [m, n] = dijkstra<int, double, double, Graph<int, double, double>>(graph, 2, 1);
        cout << m << endl;
        
        for(int key : n)
        {
            cout << key << "->";
        }
        
        
        for(auto[key,node] : graph)
        {
            cout << "Вершина " << key << " соединена с:" <<" ";
            for(auto[key, weight] : node)
            {
                cout << key << "(вес ребра " << weight << " )" << " ";
            }
            cout << "\n";
        }
        cout << endl;
    }
    catch (const char* msg)
    {
        cout << msg << endl;
        
    }
    
    
    return 0;
}
