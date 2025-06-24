/**
 \function dijkstra Алгоритм  Дейкстры
 \param g граф
 \param key_from ключ начала пути
 \param key_to ключ конца пути
 \return пару из длинны пути и сам пути по узлам
 */
template <typename key_type, typename weight_type, typename value_type, typename graph_t>
pair<weight_type, vector<key_type>> dijkstra(const graph_t& g, key_type key_from, key_type key_to)
{
    
    graph_t g_copy;
    g_copy = g;
    for(auto[key, node] : g_copy)
    {
        if(key == key_from) g_copy[key] = 0;
        else g_copy[key] = LONG_MAX;
    }
    
    try
    {
        g_copy.at(key_from);
    }
    catch (const char* msg)
    {
        throw msg;
    }
    
    try
    {
        g_copy.at(key_to);
    }
    catch (const char* msg)
    {
        throw msg;
    }
    
    for(auto [key, node] : g_copy)
    {
        for(auto [key_2, weight] : node)
        {
            if(weight < 0) throw "Есть отрицательное ребро";
        }
    }
    vector<key_type> visited;
    visited.push_back(key_from);
    //weight_type P[g_copy.size()];
    map<key_type, key_type> route;
    key_type tmp = key_from;
    map<key_type, value_type> res;
    value_type min;
    
    while (visited.size() < g_copy.size())
    {
        for(auto [key, node] : g_copy)
        {
            if(key == tmp)
            {
                for(auto[key_2, weight] : node)
                {
                    if(g_copy[key_2] > g_copy[key] + weight)
                    {
                        g_copy[key_2] = g_copy[key] + weight;
                        res[key_2] = g_copy[key_2];
                        route[key_2] = key;
                    }
                }
            }
        }
        
        min = LONG_MAX;
        for(auto[key, value] : res)
        {
            if(min >= value && count(visited.begin(), visited.end(), key) == 0)
            {
                min = value;
                tmp = key;
            }
        }
        visited.push_back(tmp);
    }
    /*
    for(auto[key, node] : g_copy)
    {
        cout << key <<"  "<<g_copy[key] << endl;
    }
    
    cout << "\n" << endl;
    for(auto[flag1, flag2] : route)
    {
        cout << flag1 << " : " << flag2 << endl;
    }
    */
    vector<key_type> ans;
    ans.insert(ans.begin(), key_to);
    tmp = key_to;
    
    int count_ = 0;
    while(tmp != key_from)
    {
        for(auto[last, previous] : route)
        {
            if(last == tmp)
            {
                tmp = previous;
                break;
            }
        }
        //cout << tmp << endl;
        ans.insert(ans.begin(), tmp);
        count_ += 1;
        if(count_ >= g_copy.size() - 1) throw "такого пути нет";
    }
    
    return pair<weight_type, vector<key_type>>(g_copy[key_to], ans);
}

