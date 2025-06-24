#include <iomanip>
#include <typeinfo>

#include "fstream"
#include "istream"



using namespace std;
/**
 \brief Данное пространство имен содержит все функции для работы над матрицами
 
 В данном пространстве имен хранится класс матриц, над которым мы производим операции сложения, умножения, вовзведения в стпень, транспонирование, нахождение обратной и работа над матрицами с комплексными числами
 */
namespace linalg {

    template<typename T = double>
    class Matrix {
    public:
        //может быть надо будет исправить на public
        int m_rows;
        int m_columns;
        T **m_ptr;
        //конструкторы
        Matrix<T>(int m_r, int m_c = 1) {
            m_rows = m_r;
            m_columns = m_c;
            m_ptr = new T *[m_rows];
            for (int i = 0; i < m_rows; i++) {
                m_ptr[i] = new T[m_columns];
            }
        }

        //конструктор копирования
        Matrix<T>(const Matrix &m) {
            m_rows = m.m_rows;
            m_columns = m.m_columns;
            
            m_ptr = new T *[m_rows];
            for (int i = 0; i < m_rows; i++) {
                m_ptr[i] = new T[m_columns];
            };
            
            for(int i = 0; i < m_rows; i++)
            {
                for(int j = 0; j < m_columns; j++)
                {
                    m_ptr[i][j] = m.m_ptr[i][j];
                }
            }
            //m_ptr = m.m_ptr;
        }

        //констрктор с помощью инициализаторов
        Matrix<T>(initializer_list<vector<T>> l) noexcept {
            m_rows = l.size();
            m_ptr = new T *[m_rows];

            m_rows = 0;
            for (vector<T> n: l) {
                m_columns = n.size();
                m_ptr[m_rows] = new T[m_columns];
                m_rows += 1;
            }
//{{1}, {2}, {4}, {5}} == {1, 2 ,3 ,4}
            m_rows = 0;
            m_columns = 0;

            for (vector<T> n: l) {
                m_columns = 0;
                for (T h: n) {
                    m_ptr[m_rows][m_columns] = h;
                    m_columns += 1;
                }
                m_rows += 1;
            }
        }

        //констрктор с помощью инициализаторов
        Matrix(initializer_list<T> l) noexcept {
            m_rows = l.size();
            m_ptr = new T *[m_rows];
            for (int i = 0; i < m_rows; i++) {
                m_ptr[i] = new T[1];
            }

            m_rows = 0;
            for (T n: l) {
                m_ptr[m_rows][0] = n;
                m_rows += 1;
            }
            m_columns = 1;
        }

        //деструктор
        ~Matrix() {
            for (int i = 0; i < m_rows; i++) {
                delete[] m_ptr[i];
            }

            delete[] m_ptr;
        }

        //конструктор перемещения
        Matrix<T>(Matrix &&m) noexcept {
            m_rows = m.m_rows;
            m_columns = m.m_columns;
            
            m_ptr = new T *[m_rows];
            for (int i = 0; i < m_rows; i++) {
                m_ptr[i] = new T[m_columns];
            };
            
            for(int i = 0; i < m_rows; i++)
            {
                for(int j = 0; j < m_columns; j++)
                {
                    m_ptr[i][j] = m.m_ptr[i][j];
                }
            }
            //m_ptr = m.m_ptr;
            //m.m_ptr = nullptr;
            m.m_rows = 0;
            m.m_columns = 0;

        }
        //перегрузка операторов

        friend std::ostream& operator<<(std::ostream &os, const Matrix<T>& m1)
        {
            for (int i = 0; i < m1.m_rows; i++)
            {
                os << "|";
                for (int j = 0; j < m1.m_columns; j++)
                {
                    os << setw(20) << m1.m_ptr[i][j];
                }
                os << "|";
                os << "\n";
            }
            
            return os;
        }
        
        //перегрузка операции +=
        Matrix<T> &operator+=(const Matrix<T> &m1) {
            try
            {
                if (m1.m_rows != m_rows || m1.m_columns != m_columns) {
                    throw 1;
                }
                
                for (int i = 0; i < m1.m_rows; i++) {
                    for (int j = 0; j < m1.m_columns; j++) {
                        m_ptr[i][j] = m_ptr[i][j] + m1.m_ptr[i][j];
                    }
                }
                return *this;
            }
            catch(int exit_value)
            {
                cout << "Ошибка" << endl;
                exit(-1);
            }
        }

        //перегрузка оператора -=
        Matrix<T> &operator-=(const Matrix<T> &m1) {
            try
            {
                if (m1.m_rows != m_rows && m1.m_columns != m_columns) {
                    throw 1;
                }
                
                for (int i = 0; i < m1.m_rows; i++) {
                    for (int j = 0; j < m1.m_columns; j++) {
                        m_ptr[i][j] = m_ptr[i][j] - m1.m_ptr[i][j];
                    }
                }
                return *this;
            }
            catch(int exit_value)
            {
                cout << "Ошибка" << endl;
                exit(-1);
            }
        }

        //перемножение матрицы на число справа
        Matrix<T> operator*(T k) {
            Matrix<T> res(this->m_rows, this->m_columns);
            for (int i = 0; i < m_rows; i++) {
                for (int j = 0; j < m_columns; j++) {
                    res.m_ptr[i][j] = m_ptr[i][j] * k;
                }
            }
            return (res);
        }

        //перемножение матрицы на число слева
        friend Matrix<T> operator*(T k, const Matrix<T> &m1) {
            Matrix<T> res(m1.m_rows, m1.m_columns);
            for (int i = 0; i < m1.m_rows; i++) {
                for (int j = 0; j < m1.m_columns; j++) {
                    res.m_ptr[i][j] = m1.m_ptr[i][j] * k;
                }
            }
            return (res);
        }

        //перегрузка оператора +
        Matrix operator+(const Matrix<T> &m1) {
            try
            {
                if (m1.m_rows != m_rows || m1.m_columns != m_columns) {
                    throw 1;
                }
                
                Matrix<T> res(m1.m_rows, m1.m_columns);
                for (int i = 0; i < m1.m_rows; i++) {
                    for (int j = 0; j < m1.m_columns; j++) {
                        res.m_ptr[i][j] = m_ptr[i][j] + m1.m_ptr[i][j];
                    }
                }
                return (res);
            }
            
            catch(int exit_value)
            {
                cout << "Ошибка" << endl;
                exit(-1);
            }
            
        }

        //перегрузка оператора -
        Matrix operator-(const Matrix<T> &m1) {
            try
            {
                if (m1.m_rows != m_rows || m1.m_columns != m_columns) {
                    throw 1;
                }
                
                Matrix<T> res(m1.m_rows, m1.m_columns);
                for (int i = 0; i < m1.m_rows; i++) {
                    for (int j = 0; j < m1.m_columns; j++) {
                        res.m_ptr[i][j] = m_ptr[i][j] - m1.m_ptr[i][j];
                    }
                }
                return (res);
            }
            
            catch(int exit_value)
            {
                cout << "Ошибка" << endl;
                exit(-1);
            }
        }

        //перегрузка оператора * матриц
        // первая l*m, вторая m*n, если это не так, то матрицы нельзя перемножить
        Matrix<T> operator*(const Matrix<T> &m1) {
            try
            {
                if (m1.m_rows != m_columns) {
                    throw 1;
                }
                
                T value = 0;
                Matrix<T> res(m_rows, m1.m_columns);
                for (int i = 0; i < m_rows; i++) {
                    for (int j = 0; j < m1.m_columns; j++) {
                        for (int r = 0; r < m_columns; r++) {
                            value += m_ptr[i][r] * m1.m_ptr[r][j];
                            //res.m_ptr[i][j] += m_ptr[i][r] * m1.m_ptr[r][j];
                        }
                        res.m_ptr[i][j] = value;
                        value = 0;
                    }
                }
                return res;
            }
            
            catch(int exit_value)
            {
                cout << "Ошибка" << endl;
                exit(-1);
            }
        }

        // перегрузка оператора *=
        Matrix<T> operator*=(double a)
        {
            for(int i = 0; i < m_rows; i++)
            {
                for(int j = 0; j < m_columns; j++)
                {
                    m_ptr[i][j] *= a;
                }
            }
            
            return *this;
            
        }
        

        //перегрузка опретора =
        Matrix<T> operator=(const Matrix<T>& m1)
        {
            m_rows = m1.m_rows;
            m_columns = m1.m_columns;
            for(int i = 0; i < m_rows; i++)
            {
                for(int j = 0; j < m_columns; j++)
                {
                    m_ptr[i][j] = m1.m_ptr[i][j];
                }
            }
            //m_ptr = m1.m_ptr;

            return *this;
        }
        
        //определитель методом гаусса
        T det()
        {
            try
            {
                if(m_rows != m_columns){
                    throw 1;
                }
                //создаем матрицу с элементами исходной
                double** mat = new double *[m_rows];
                for (int i = 0; i < m_rows; i++) {
                    mat[i] = new double[m_columns];
                }
                
                for (int i = 0; i < m_rows; i++)
                {
                    for (int j = 0; j < m_columns; j++)
                    {
                        mat[i][j] = double(m_ptr[i][j]);
                    }
                }
                
                double determinat = 1.0;
                int y = 0;
                
                //изменяем матрицу методом Гауса
                for(int t = 0; t < m_rows - 1; t++)
                {
                    y = 0;
                    if(mat[t][t] == 0)
                    {
                        for(int i = t + 1; i < m_rows; i ++)
                        {
                            if(mat[i][t] != 0)
                            {
                                double *tmp = mat[i];
                                mat[i] = mat[t];
                                mat[t] = tmp;
                                determinat *= -1;
                                break;
                            }
                            y += 1;
                        }
                    }
                    
                    if(y == m_rows - t - 1)
                    {
                        determinat = 0;
                        return determinat;
                    }
                    //y = 0;
                    double a_1 = mat[t][t];
                    
                    for(int i = t + 1; i < m_rows; i++)
                    {
                        double tmp1 = mat[i][t] / a_1;
                        for(int j = t; j < m_columns; j++)
                        {
                            mat[i][j] = mat[i][j] - tmp1 * mat[t][j];
                            if(abs(mat[i][j]) < 0.00000000001) mat[i][j] = 0;
                        }
                    }
                }
                
                //перемножаем элементы стоящие на главной диагонали
                for(int i = 0; i < m_rows; i++)
                {
                    determinat *= mat[i][i];
                }
                
                if(determinat == -0)
                {
                    determinat = 0;
                }
                
                if(T(determinat) != determinat)
                {
                    determinat = round(determinat);
                }
                //cout << int;
                
                return determinat;
            }
            
            catch(int exit_value)
            {
                cout << "Ошибка" << endl;
                exit(-1);
            }
        }
        
        
        
        
        
        
        
        
        
        
        
        
        int rank()
        {
            if(this->det() != 0) return min(m_rows, m_columns);
            //создаем матрицу с элементами исходной
            double** mat = new double *[m_rows];
            for (int i = 0; i < m_rows; i++) {
                mat[i] = new double[m_columns];
            }

            for (int i = 0; i < m_rows; i++)
            {
                for (int j = 0; j < m_columns; j++)
                {
                    mat[i][j] = double(m_ptr[i][j]);
                }
            }

            int dim = min(m_rows, m_columns);
            int y = 0;
            int g = 0;

            //изменяем матрицу методом Гауса
            //loop:
            for(int t = 0; t < m_rows - 1; t++)
            {
                loop:
                y = 0;
                
                if(mat[t][t] == 0)
                {
                    for(int i = t + 1; i < m_rows; i ++)
                    {
                        if(mat[i][t] != 0)
                        {
                            double *tmp = mat[i];
                            mat[i] = mat[t];
                            mat[t] = tmp;
                            //determinat *= -1;
                            break;
                        }
                        y += 1;
                    }
                }
                
                if(y == m_rows - t - 1)
                {
                    g += 1;
                    if(t < m_rows - 1)
                    {
                        t += 1;
                        goto loop;
                    }
                    else return (dim - g);
                }
                double a_1 = mat[t][t];

                for(int i = t + 1; i < m_rows; i++)
                {
                    double tmp1 = mat[i][t] / a_1;
                    for(int j = t; j < m_columns; j++)
                    {
                        mat[i][j] = mat[i][j] - tmp1 * mat[t][j];
                        if(abs(mat[i][j]) < 0.000000001) mat[i][j] = 0;
                    }
                }
            }
            
            
            int b = 0;
            int rg = min(m_rows, m_columns);
            
            for(int r = 0; r < m_rows; r++)
            {
                for(int k = 0; k < m_columns; k++)
                {
                    if(mat[r][k] == 0) b += 1;
                }
                
                if(b == m_columns) rg -= 1;
            }
            
            return rg;
        }
        
        
        
        
        
        
        
        
        

        //нахождение ранга матрицы
        //используется метод Гаусса, но с выбором опорного элемента
        
        /*int rank()
        {
            double e = 0.0000000000001;
            
            int n = m_rows;
            int m = m_columns;
            
            double** a = new double *[m_rows];
            for (int i = 0; i < m_rows; i++) {
                a[i] = new double[m_columns];
            }
            
            int rg = max(n,m);
            
            //данный вектор показывает использование строки(если мы не использовали ее но возвратится 0, иначе - 1)
            vector<char> line_used (n);
            
            for (int i=0; i<m; ++i)
            {
                int j;
                for (j=0; j<n; ++j)
                {
                    if (!line_used[j] && abs(a[j][i]) > e)  break;
                }
                
                if (j == n) --rg;
                else
                {
                    line_used[j] = true;
                    
                    for (int p=i+1; p<m; ++p)
                    {
                        a[j][p] /= a[j][i];
                    }
                    
                    for (int k=0; k<n; ++k)
                    {
                        if (k != j && abs (a[k][i]) > e)
                        {
                            for (int p=i+1; p<m; ++p)
                            {
                                a[k][p] -= a[j][p] * a[k][i];
                            }
                        }
                    }
                }
            }
            
            for(int i = 0; i < m_rows; i++)
            {
                for(int j = 0; j < m_columns; j++)
                {
                    cout << a[i][j] << "\t";
                }
                cout << "\n";
            }
            
            return rg;
        }*/

        //нахождение нормы Фробениуса(Евклидовой формы)
        double norm()
        {
            double norm_f = 0;

            for(int i = 0; i < m_rows; i++)
            {
                for(int j = 0; j < m_columns; j++)
                {
                    norm_f += pow(m_ptr[i][j], 2);
                }
            }

            norm_f = pow(norm_f ,0.5);

            return norm_f;
        }


        //след матрицы
        double trace()
        {
            double m_trace = 0;

            for(int i = 0; i < m_rows; i++)
            {
                m_trace += m_ptr[i][i];
            }

            return m_trace;
        }

        //транспонирование матрицы
        friend Matrix<T> transpose(const Matrix<T>& m)
        {
            Matrix<T> res(m.m_rows, m.m_columns);
            for(int i = 0; i < m.m_rows; i++)
            {
                for(int j = 0; j < m.m_columns; j++)
                {
                    res.m_ptr[i][j] = m.m_ptr[j][i];
                }
            }

            return res;
        }

        friend Matrix<T> power(Matrix<T>& m, int a)
        {
            Matrix<T> res(m);
            
            for(int i = 0; i < a - 1; i++)
            {
                res = res * m;
            }
            
            return res;
        }
        
        //удаление i-й строки и j-го столбца
        friend T minor(const Matrix<T>& m, int a, int b)
        {
            Matrix<T> m2(m.m_rows - 1, m.m_columns - 1);
            
            for(int i = 0; i < m.m_rows - 1; i++)
            {
                for(int j = 0; j < m.m_columns - 1; j ++)
                {
                    if(i < a)
                    {
                        if(j < b) m2.m_ptr[i][j] = m.m_ptr[i][j];
                        else      m2.m_ptr[i][j] = m.m_ptr[i][j + 1];
                    }
                    
                    else
                    {
                        if(j < b) m2.m_ptr[i][j] = m.m_ptr[i + 1][j];
                        else      m2.m_ptr[i][j] = m.m_ptr[i + 1][j + 1];
                    }
                }
            }
            
            T tmp = m2.det();
            tmp *= pow(-1, a + b);
            
            return tmp;
        }
        //обратная матрица
        friend Matrix<T> inv(const Matrix<T>& m)
        {
            try
            {
                if(m.det() == 0)
                {
                    throw 1;
                }
                Matrix<T> inverse_(m.m_rows, m.m_columns);
                Matrix<T> m1(m);
                
                for(int i = 0; i < m.m_rows; i++)
                {
                    for(int j = 0; j < m.m_columns; j++)
                    {
                        inverse_.m_ptr[i][j] = minor(m1, i, j);
                    }
                }
                
                Matrix<T> inverse = transpose(inverse_);
                double tmp = 1/m1.det();
                inverse *= tmp;
                
                return inverse;
            }
            
            catch(int exit_value)
            {
                cout << "Ошибка" << endl;
                exit(-1);
            }
        }
        
        
        int rows() const
        {
            return m_rows;
        }
        
        int col() const
        {
            return m_columns;
        }
         
    };


template<typename T> struct TextMode {};
template<>
struct TextMode<double>{
public:
    static void write(string file_name, const Matrix<double>& obj)
    {
        ofstream out;
        string s = typeid(obj).name();
        /*if(s == "N6linalg6MatrixIiEE") s = "int";
        else if (s == "N6linalg6MatrixIdEE") s = "double";
        else if (s == "N6linalg6MatrixI7ComplexIiEEE") s = "Complex<int>";
        else s = "Complex<double>";*/
        
        out.open(file_name, ios::app);
        if(out.is_open())
        {
            out << "type: " << "double" << endl;
            out << "rows: " << obj.rows() << endl;
            out << "columns: " << obj.col() << endl;
            out << obj;
        }
    }
    
    static Matrix<double> read(string file_name)
    {
        ifstream in(file_name);
        string n_str;
        string m_str;
        string type_mat;
        
        getline(in, type_mat);
        
        in.seekg(6*sizeof(char), ios::cur);
        getline(in, n_str);
        
        in.seekg(8*sizeof(char), ios::cur);
        getline(in, m_str);
        
        int n = atoi(n_str.c_str());
        int m = atoi(m_str.c_str());
        
        
        double** a = new double *[n];
        for (int i = 0; i < n; i++) {
            a[i] = new double[m];
        }
        
        string line;
        char buf[100];
        
        if(in.is_open())
        {
            for(int i = 0; i < n; i++)
            {
                in.seekg(1*sizeof(char), ios::cur);
                for(int j = 0; j < m; j++)
                {
                    in.get(buf, 21);
                    a[i][j] = atof(buf);
                }
                in.seekg(2*sizeof(char), ios::cur);
            }
        }
        
        Matrix<double> mat1(n, m);
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < m; j++)
            {
                mat1.m_ptr[i][j] = a[i][j];
            }
        }
        
        return mat1;
    }
    
};






//template<typename T = double>
template<>
struct TextMode<int>{
public:
    static void write(string file_name, const Matrix<int>& obj)
    {
        ofstream out;
        /*string s = typeid(obj).name();
        if(s == "N6linalg6MatrixIiEE") s = "int";
        else if (s == "N6linalg6MatrixIdEE") s = "double";
        else if (s == "N6linalg6MatrixI7ComplexIiEEE") s = "Complex<int>";
        else s = "Complex<double>";*/
        
        out.open(file_name, ios::app);
        if(out.is_open())
        {
            out << "type: " << "int" << endl;
            out << "rows: " << obj.rows() << endl;
            out << "columns: " << obj.col() << endl;
            out << obj;
        }
    }
    
    static Matrix<int> read(string file_name)
    {
        ifstream in(file_name);
        string n_str;
        string m_str;
        string type_mat;
        
        getline(in, type_mat);
        
        in.seekg(6*sizeof(char), ios::cur);
        getline(in, n_str);
        
        in.seekg(8*sizeof(char), ios::cur);
        getline(in, m_str);
        
        int n = atoi(n_str.c_str());
        int m = atoi(m_str.c_str());
        
        
        int** a = new int *[n];
        for (int i = 0; i < n; i++) {
            a[i] = new int[m];
        }
        
        string line;
        char buf[100];
        
        if(in.is_open())
        {
            for(int i = 0; i < n; i++)
            {
                in.seekg(1*sizeof(char), ios::cur);
                for(int j = 0; j < m; j++)
                {
                    in.get(buf, 21);
                    a[i][j] = atoi(buf);
                }
                in.seekg(2*sizeof(char), ios::cur);
            }
        }
        
        Matrix<int> mat1(n, m);
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < m; j++)
            {
                mat1.m_ptr[i][j] = a[i][j];
            }
        }
        
        return mat1;
    }
    
};

template<>
struct TextMode<Complex<int>>{
public:
    static void write(string file_name, const Matrix<Complex<int>>& obj)
    {
        ofstream out;
        /*string s = typeid(obj).name();
        if(s == "N6linalg6MatrixIiEE") s = "int";
        else if (s == "N6linalg6MatrixIdEE") s = "double";
        else if (s == "N6linalg6MatrixI7ComplexIiEEE") s = "Complex<int>";
        else s = "Complex<double>";*/
        
        out.open(file_name, ios::app);
        if(out.is_open())
        {
            out << "type: " << "Complex<int>" << endl;
            out << "rows: " << obj.rows() << endl;
            out << "columns: " << obj.col() << endl;
            out << obj;
        }
    }
    
    static Matrix<Complex<int>> read(string file_name)
    {
        ifstream in(file_name);
        string n_str;
        string m_str;
        string type_mat;
        
        getline(in, type_mat);
        
        in.seekg(6*sizeof(char), ios::cur);
        getline(in, n_str);
        
        in.seekg(8*sizeof(char), ios::cur);
        getline(in, m_str);
        
        int n = atoi(n_str.c_str());
        int m = atoi(m_str.c_str());
        
        string line;
        char buf[100];
        
        Complex<int>** arr = new Complex<int> *[n];
            for (int i = 0; i < n; i++) {
                arr[i] = new Complex<int>[m];
            }
            
        if(in.is_open())
        {
            for(int i = 0; i < n; i++)
            {
                in.seekg(1*sizeof(char), ios::cur);
                for(int j = 0; j < m; j++)
                {
                    in.get(buf, 21);
                    int real = atoi(buf);
                    char* str_ = new char[25];
                    
                    strcpy(str_, &buf[strLen(buf) + int(to_string(real).size())]);
                    //cout << str_ << endl;
                    int imag = atoi(str_);
                    
                    if(buf[strLen(buf) + int(to_string(real).size())] == '*')
                    {
                        real = 0;
                        imag = atoi(buf);
                    }
                    arr[i][j] = Complex<int>(real, imag);
                    delete [] str_;
                }
                in.seekg(2*sizeof(char), ios::cur);
            }
        }
        
        Matrix<Complex<int>> mat(n, m);
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < m; j++)
            {
                mat.m_ptr[i][j] = arr[i][j];
            }
        }
        
        return mat;
    }
};

template<>
struct TextMode<Complex<double>>{
public:
    static void write(string file_name, const Matrix<Complex<double>>& obj)
    {
        ofstream out;
        /*string s = typeid(obj).name();
        if(s == "N6linalg6MatrixIiEE") s = "int";
        else if (s == "N6linalg6MatrixIdEE") s = "double";
        else if (s == "N6linalg6MatrixI7ComplexIiEEE") s = "Complex<int>";
        else s = "Complex<double>";*/
        
        out.open(file_name, ios::app);
        if(out.is_open())
        {
            out << "type: " << "Complex<double>" << endl;
            out << "rows: " << obj.rows() << endl;
            out << "columns: " << obj.col() << endl;
            out << obj;
        }
    }
    
    static Matrix<Complex<double>> read(string file_name)
    {
        ifstream in(file_name);
        string n_str;
        string m_str;
        string type_mat;
        
        getline(in, type_mat);
        
        in.seekg(6*sizeof(char), ios::cur);
        getline(in, n_str);
        
        in.seekg(8*sizeof(char), ios::cur);
        getline(in, m_str);
        
        int n = atoi(n_str.c_str());
        int m = atoi(m_str.c_str());
        
        string line;
        char buf[100];
        
        Complex<double>** arr = new Complex<double> *[n];
            for (int i = 0; i < n; i++) {
                arr[i] = new Complex<double>[m];
            }
            
        if(in.is_open())
        {
            for(int i = 0; i < n; i++)
            {
                in.seekg(1*sizeof(char), ios::cur);
                for(int j = 0; j < m; j++)
                {
                    in.get(buf, 21);
                    int real = atoi(buf);
                    char* str_ = new char[25];
                    
                    strcpy(str_, &buf[strLen(buf) + int(to_string(real).size())]);
                    //cout << str_ << endl;
                    int imag = atoi(str_);
                    
                    if(buf[strLen(buf) + int(to_string(real).size())] == '*')
                    {
                        real = 0;
                        imag = atoi(buf);
                    }
                    arr[i][j] = Complex<double>(real, imag);
                    delete [] str_;
                }
                in.seekg(2*sizeof(char), ios::cur);
            }
        }
        
        Matrix<Complex<double>> mat(n, m);
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < m; j++)
            {
                mat.m_ptr[i][j] = arr[i][j];
            }
        }
        
        return mat;
    }
};

template <typename T>
struct BinaryMode
{
public:
    static void write(string file_name, const Matrix<T>& obj)
    {
        ofstream out(file_name, ios::binary | ios::app );
        string s = typeid(obj).name();
        if(s == "N6linalg6MatrixIiEE") s = "int";
        else if (s == "N6linalg6MatrixIdEE") s = "double";
        else if (s == "N6linalg6MatrixI7ComplexIiEEE") s = "Complex<int>";
        else s = "Complex<double>";
        
        //out.open(file_name, ios::app);
        if(out.is_open())
        {
            out << "type: " << s << endl;
            out << "rows: " << obj.rows() << endl;
            out << "columns: " << obj.col() << endl;
            out << obj;
        }
    }
};


};
