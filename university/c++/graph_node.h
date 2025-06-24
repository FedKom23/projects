using namespace std;
/**
\class Graph
 Данный класс является графом хранящим в себе данные о матрице смежности, ключах (именах вершин), значениях (то, что xранит в себе каждая вершина и весах (вес каждого ребра)
*/
template <typename key_type,typename value_type, typename weight_type>
class Graph
{
public:
    /**
    \class Node
     Хранит данные об узле
    */
    class Node
    {
    private:
        value_type val;
        map<key_type, weight_type> edge;//edge - ребра, которые связывают данный узел с другими (ключи и веса ребер)
    public:
        /**
         \brief дефолтный конструктор
         */
        Node() = default;
        /**
         \brief конструктор
         \param val_1 значение, которое будет лежать в узле
         */
        Node(value_type val_1)
        {
            //edge.clear();
            val = val_1;
        }
        
        /**
         \brief конструктор копирования
         */
        Node(const Node& node)
        {
            val = node.val;
            for(auto it = node.edge.begin(); it != node.edge.end(); it++)
            {
                edge.insert(pair<key_type, weight_type>(it->first, it->second));
            }
        }
        
        
        /**
         \brief конструктор пермещения
         */
        Node(Node&& node) = default;
        
        /**
         \function operator= оператор присваивания
         \param node узел, который копируем
         */
        Node& operator=(const Node& node)
        {
            val = node.val;
            for(auto it = node.edge.begin(); it != node.edge.end(); it++)
            {
                edge.insert(pair<key_type, weight_type>(it->first, it->second));
            }
            
            return *this;
        }
        
        /**
         \function operator= оператор присваивания
         \param node узел, который копируем и потом удаляем
         */
        Node& operator=(Node&& node)
        {
            val = node.val;
            for(auto it = node.edge.begin(); it != node.edge.end(); it++)
            {
                edge.insert(pair<key_type, weight_type>(it->first, it->second));
            }
            
            node.edge.clear();
            node.val = 0;
            
            return *this;
        }
        
        /**
         \function empty
         \return bool пустой ли набор узлов
         */
        bool empty()
        {
            if(edge.size() != 0) return false;
            else return true;
        }
        /**
         \function size
         \return кол-во узлов у графа
         */
        size_t size()
        {
            return edge.size();
        }
        /**
         \function value
         \return ссылка на хранимое в узле значение
         */
        value_type &value()
        {
            return val;
        }
        /**
         \function clear ничего не возвращает. Удаляет все рёбра, исходящие из этого узла
         */
        void clear()
        {
            edge.clear();
        }
        
        using iterator = typename map<key_type, weight_type>::iterator;
        using const_iterator = typename map<key_type, weight_type>::const_iterator;
        /**
         \function begin
         \return возвращает итератор на начало
         */
        iterator begin()
        {
            iterator it = edge.begin();
            return it;
        }
        /**
         \function end
         \return возвращает итератор на конец
         */
        iterator end()
        {
            iterator it = edge.end();
            return it;
        }
        /**
         \function сbegin
         \return возвращает итератор на начало константного графа
         */
        iterator cbegin()
        {
            const_iterator it = edge.cbegin();
            return it;
        }
        /**
         \function cend
         \return возвращает итератор на конец константного графа
         */
        iterator cend()
        {
            const_iterator it = edge.cend();
            return it;
        }
        
        /*
        weight_type weight_t(key_type key)
        {
            weight_type tmp;
            for(auto it = edge.begin(); it != edge.end(); it++)
            {
                if(it->first == key)
                {
                    tmp = it->second;
                }
            }
            return tmp;
        }
         */
        /**
         \function erase_edge
         
         \param key ключ, с которым соединен данный узел
         \return успешное ли удаление ребра, который исходит из этого узла и заканчивается в том, который называется key
         */
        bool erase_edge(key_type key)
        {
            for(auto it = edge.begin(); it != edge.end(); it++)
            {
                if(it->first == key)
                {
                    edge.erase(it);
                    return true;
                }
            }
            
            return false;
        }
        /**
         \function insert_edge
         
         \param key ключ, с которым хотим соединить узел
         \param weight вес, который будет иметь указанное ребро
         \return вернёт пару: [iterator, bool], состоящую из итератора на key и bool переменной (смогли  добавить ребро или нет)
         */
        pair<iterator, bool> insert_edge(key_type key, weight_type weight)
        {
            return edge.insert(pair<key_type, weight_type>(key, weight));
        }
        
        /**
         \function insert_or_assign_edge
         
         \param key ключ, с которым хотим соединить узел
         \param weight вес, который будет иметь указанное ребро
         \return вернёт пару: [iterator, bool], состоящую из итератора на key и bool переменной (смогли  добавить ребро или нет)
         */
        pair<iterator, bool> insert_or_assign_edge(key_type key, weight_type weight)
        {
            return edge.insert_or_assign(key, weight);
        }
    };
    
    map<key_type, Node> graph;
    /**
     \brief дефолтный конструктор
     */
    Graph<key_type, value_type, weight_type>()
    {
        graph.clear();
    }
    
    /**
     \brief конструктор копирования
     */
    Graph<key_type, value_type, weight_type>(const Graph& g)
    {
        graph.clear();
        for(auto it = g.graph.begin(); it != g.graph.end(); it++)
        {
            graph.insert(pair<key_type, Node>(it->first, Node(it->second)));
        }
    }
    /**
     \brief конструктор перемещения
     */
    Graph<key_type, value_type, weight_type>(Graph&& g)
    {
        graph.clear();
        for(auto it = g.graph.begin(); it != g.graph.end(); it++)
        {
            graph.insert(pair<key_type, Node>(it->first, Node(move(it->second))));
        }
        g.graph.clear();
    }
    /**
     \function operator= дефолтный оператор присваивания
     */
    Graph<key_type, value_type, weight_type> operator=(const Graph& g)
    {
        graph.clear();
        for(auto it = g.graph.begin(); it != g.graph.end(); it++)
        {
            graph.insert(pair<key_type, Node>(it->first, Node(it->second)));
        }
        
        return *this;
    }
    
    /**
     \function operator=  оператор присваивания, который удаляет данные у графа, который мы перемещаем
     */
    Graph<key_type, value_type, weight_type> operator=(Graph&& g)
    {
        graph.clear();
        for(auto it = g.graph.begin(); it != g.graph.end(); it++)
        {
            graph.insert(pair<key_type, Node>(it->first, Node(it->second)));
        }
        g.graph.clear();
        
        return *this;
    }
    /**
     \function empty
     \return bool пустой ли набор рёбер
     */
    bool empty()
    {
        if(graph.size() != 0) return false;
        else return true;
    }
    /**
     \function size
     \return кол-во рёбер исходящих из этого узла
     */
    size_t size()
    {
        return graph.size();
    }
    /**
     \function clear ничего не возвращает. Удаляет все рёбра, исходящие из этого узла
     */
    void clear()
    {
        graph.clear();
    }
    /**
     \function swap метод класса
     \param g меняем местами граф g и исходный
     */
    void swap(Graph g)
    {
        Graph<key_type, value_type, weight_type> tmp;
        tmp = this;
        this = g;
        g = tmp;
        tmp.clear();
    }
    /**
     \function swap дружественная функция
     \param g1 первый элемент, который меняем местом со вторым
     \param g2 второй элемент, который меняем местом с первым
     */
    friend void swap(Graph g1, Graph g2)
    {
        Graph<key_type, value_type, weight_type> tmp;
        tmp = g1;
        g1 = g2;
        g2 = tmp;
    }
    
    using iterator = typename map<key_type, Node>::iterator;
    using const_iterator = typename map<key_type, Node>::const_iterator;
    /**
     \function begin
     \return итератор на начало графа
     */
    iterator begin()
    {
        iterator it = graph.begin();
        return it;
    }
    /**
     \function end
     \return итератор на конец графа
     */
    iterator end()
    {
        iterator it = graph.end();
        return it;
    }
    /**
     \function cbegin
     \return итератор на начало константного графа
     */
    iterator cbegin()
    {
        const_iterator it = graph.cbegin();
        return it;
    }
    /**
     \function cend
     \return итератор на конец константного графа
     */
    iterator cend()
    {
        const_iterator it = graph.cend();
        return it;
    }
    /**
     \function degree_in
     
     \param key узел
     \return кол-во узлов, входящих в key
     */
    size_t degree_in(key_type key)
    {
        size_t n = 0;
        for(auto it1 = graph.begin(); it1 != graph.end(); it1++)
        {
            for(auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
            {
                if(it2->first == key) n++;
            }
        }
        
        return n;
    }
    /**
     \function degree_in
     
     \param key узел
     \return кол-во узлов, выходящих из key
     */
    size_t degree_out(key_type key)
    {
        size_t n = 0;
        for(auto it = graph.begin(); it != graph.end(); it++)
        {
            if(it->first == key)
            {
                n = it->second.size();
            }
        }
        
        return n;
    }
    /**
     \function loop
     
     \param key узел
     \return есть ли цикл у узла
     */
    bool loop(key_type key)
    {
        for(auto it1 = graph.begin(); it1 != graph.end(); it1++)
        {
            if(it1->first == key)
            {
                for(auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
                {
                    if(it2->first == key) return true;
                }
            }
            else continue;
        }
        
        return false;
    }
    /**
     \function operator[]
     \param key узел
     \return возвращает ссылку на значение узла key
     */
    value_type &operator[](key_type key)
    {
        for(auto it = graph.begin(); it != graph.end(); it++)
        {
            if(it->first == key)
            {
                return it->second.value();
            }
        }
        
        Node node;
        graph.insert(pair<key_type, Node>(key, node));
        iterator it1;
        for(auto it = graph.begin(); it != graph.end(); it++)
        {
            if(it->first == key)
            {
                it1 = it;
                break;
            }
        }
        
        return it1->second.value();
        throw "Граф является константным";
        
    }
    /**
     \function at
     \param key узел
     \return возвращает ссылку на значение узла key
     */
    value_type &at(key_type key)
    {
        for(auto it = graph.begin(); it != graph.end(); it++)
        {
            if(it->first == key)
            {
                return it->second.value();
                
            }
        }
        throw "Данного элемента нет в графе";
    }
    /**
     \function insert_node
     
     \param key узел
     \param val значение
     \return пару из указателя на новый узел key  и bool переменную (добавился ли узел или нет)
     */
    pair<iterator, bool> insert_node(key_type key, value_type val)
    {
        Node node(val);
        return graph.insert(pair<key_type, Node>(key, node));
    }
    /**
     \function insert_or_assign_node
     
     \param key узел
     \param val значение
     \return пару из указателя на новый узел key  и bool переменную (добавился ли узел или нет)
     */
    pair<iterator, bool> insert_or_assign_node(key_type key, value_type val)
    {
        Node node(val);
        return graph.insert_or_assign(key, node);
    }
    /**
     \function insert_edge
     
     \param keys вектор из узлов
     \param weight вес ребра
     \return пару из указателя на новый узел key  и bool переменную (добавилось ребро в граф или нет)
     */
    pair<iterator, bool> insert_edge(vector<key_type> keys, weight_type weight)
    {
        int tmp = 0;
        iterator it_graph;
        pair<iterator, bool> p;
        for(auto it = graph.begin(); it != graph.end(); it++)
        {
            if(it->first == keys.front())
            {
                tmp += 1;
            }
            
            if (it->first == keys.back())
            {
                tmp += 1;
            }
        }
        
        if(tmp != 2)
        {
            throw "Данные вершины нельзя соеденить ребром";
        }
        
        for(auto it = graph.begin(); it != graph.end(); it++)
        {
            if(it->first == keys.front())
            {
                auto [flag1, flag2] = it->second.insert_edge(keys.back(), weight);
                p.first = it;
                p.second = flag2;
            }
        }
        
        return p;
    }
    /**
     \function insert_edge
     
     \param keys вектор из узлов
     \param weight вес ребра
     \return пару из указателя на новый узел key  и bool переменную (добавилось ребро в граф или нет)
     */
    pair<iterator, bool> insert_or_assign_edge(vector<key_type> keys, weight_type weight)
    {
        int tmp = 0;
        iterator it_graph;
        pair<iterator, bool> p;
        for(auto it = graph.begin(); it != graph.end(); it++)
        {
            if(it->first == keys.front())
            {
                tmp += 1;
            }
            
            if (it->first == keys.back())
            {
                tmp += 1;
            }
        }
        
        if(tmp != 2)
        {
            throw "Данные вершины нельзя соеденить ребром";
        }
        
        for(auto it = graph.begin(); it != graph.end(); it++)
        {
            if(it->first == keys.front())
            {
                auto [flag1, flag2] = it->second.insert_or_assign_edge(keys.back(), weight);
                p.first = it;
                p.second = flag2;
            }
        }
        
        return p;
    }
    /**
     \function clear_edges ничего не возвращает. Удаляет все рёбра в графе (останутся узлы)
     */
    void clear_edges()
    {
        for(auto it = graph.begin(); it != graph.end(); it++)
        {
            it->second.clear();
        }
    }
    /**
     \function erase_go_from  удалить все рёбра, выходящие из узла с ключом key
     
     \param key узел
     \return удалились ребра или нет
     */
    bool erase_go_from(key_type key)
    {
        for(auto it = graph.begin(); it != graph.end(); it++)
        {
            if(it->first == key)
            {
                it->second.clear();
                return true;
            }
        }
        
        return false;
    }
    /**
     \function erase_go_to удалить все рёбра, входящие в узел с ключом key
     
     \param key узел
     \return удалились ребра или нет
     */
    bool erase_go_to(key_type key)
    {
        int n = this->degree_in(key);
        int tmp = 0;
        for(auto it1 = graph.begin(); it1 != graph.end(); it1++)
        {
            it1->second.erase_edge(key);
            tmp += 1;
        }
        
        if(tmp == n) return true;
        else return false;
    
    }
    /**
     \function erase_node удалить узел к ключом key
     
     \param key узел
     \return удалился ли данный узел
     */
    bool erase_node(key_type key)
    {
        this->erase_go_to(key);
        for(auto it1 = graph.begin(); it1 != graph.end(); it1++)
        {
            if(it1->first == key)
            {
                graph.erase(it1);
                return true;
            }
        }
        
        return false;
    }
};
