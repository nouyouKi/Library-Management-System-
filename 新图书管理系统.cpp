#include <iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<string>
#include<map>
#include <iomanip>
using namespace std;

class Book
{
private:
    string Bookname; string PH; string ISBN;//主键，唯一不可重复
    string Author; double Price;
public:
    Book(string bn, string ph, string isbn, string a, double p)
        :Bookname(bn), PH(ph), ISBN(isbn), Author(a), Price(p) {
    }
    ~Book() {}
    //获取
    string getBookname() { return Bookname; }
    string getPh() { return PH; }
    string getISBN() { return ISBN; }
    string getAuthor() { return Author; }
    double getPrice() { return Price; }
    //赋值
    void setBookname(string b) { Bookname = b; }
    void setPh(string p) { PH = p; }
    void setISBN(string i) { ISBN = i; }
    void setAuthor(string a) { Author = a; }
    void setPrice(double p) { Price = p; }
    //展示
    void show(int total)
    {
        cout << "┌────────────────────────────────────────────────────────────┐" << endl;
        cout << left
            << "│ 书名：" << setw(12) << Bookname
            << " 出版社：" << setw(12) << PH
            << " │" << endl;
        cout << left
            << "│ ISBN：" << setw(15) << ISBN
            << " 作者：" << setw(12) << Author
            << " │" << endl;
        cout << left
            << "│ 价格：" << fixed << setprecision(1) << Price << " 元"
            << " \t\t库存量：" << total << " 本"
            << " \t\t│" << endl;
        cout << "└────────────────────────────────────────────────────────────┘" << endl;
    }
};

//功能类
class Function
{
private:
    int num; Book* booklist[10000];
    //用isbn码来查找书籍位置
    int findbookbyISBN(const string& isbn) const
    {
        for (int i = 0; i < num; ++i)
        {
            if (booklist[i] != nullptr && booklist[i]->getISBN() == isbn)
                return i;
        }
        return -1;
    }
    //统计库存量
    map<string, int> getStockmap()const
    {
        map<string, int> m;
        for (int i = 0; i < num; i++)
        {
            if (booklist[i] != nullptr)
                m[booklist[i]->getBookname()]++;
        }
        return m;//返回同名书籍对应的库存量的映射表
    }
    int getTotalByname(const string& name)const
    {
        auto stock = getStockmap();
        return stock.count(name) ? stock.at(name) : 0;//返回对应单一书籍对应的库存量
        //.count()检查这个键是否存在；.at()按照key去取值
    }
public:
    Function() :num(0) {
        // 初始化所有指针为nullptr，避免野指针
        for (int i = 0; i < 10000; i++)
            booklist[i] = nullptr;
    }
    ~Function()
    {
        for (int i = 0; i < num; i++)
            if (booklist[i] != nullptr)
                delete booklist[i];
    }
    //1.销售功能
    void shop()
    {
        string name; int buynum;
        cout << "请输入要购买的书名：";
        cin >> name;
        int total = getTotalByname(name);
        if (total == 0)
        {
            cout << "该书无库存！" << endl;
            return;
        }
        cout << "《" << name << "》当前总库存：" << total << endl;
        cout << "请输入购买数量：";
        cin >> buynum;
        if (buynum <= 0 || buynum > total)
        {
            cout << "购买数量不合法！超过库存或输入为0。" << endl;
            cout << "已退出购买流程。" << endl;
            return;
        }
        int del = 0;//从书架里删除流程
        for (int i = 0; i < num && del < buynum; )
        {
            if (booklist[i] != nullptr && booklist[i]->getBookname() == name)
            {
                delete booklist[i];
                for (int j = i; j < num - 1; j++)
                    booklist[j] = booklist[j + 1];
                num--;
                booklist[num] = nullptr; // 防止野指针，标记该位置已无有效对象
                del++;
            }
            else
                i++;
        }
        cout << "购买成功！已购买 " << buynum << " 本" << endl;
        cout << "《" << name << "》剩余库存：" << getTotalByname(name) << endl;
    }
    //2.1添加功能
    void add()
    {
        if (num >= 10000)
        {
            cout << "列表已满，不可添加。" << endl;
            return;
        }
        string bn, ph, isbn, a;
        double p;
        cout << "输入书名："; cin >> bn;
        cout << "输入出版社："; cin >> ph;
        cout << "输入ISBN码："; cin >> isbn;
        cout << "输入作者："; cin >> a;
        cout << "输入价格："; cin >> p;
        Book* b = new Book(bn, ph, isbn, a, p);
        bool isrepetitive = false;
        for (int i = 0; i < num; i++)
        {
            if (booklist[i] != nullptr && booklist[i]->getISBN() == b->getISBN())
            {
                isrepetitive = true;
                break;
            }
        }
        if (isrepetitive)
        {
            cout << "ISBN码重复，数据重复，不可添加。" << endl;
            delete b;
        }
        else
        {
            booklist[num++] = b;
            cout << "添加成功！" << endl;
        }
    }
    //2.2查询功能
    void query()
    {
        int way;
        string key;
        cout << "请输入查询方式：1.书名 2.ISBN码 3.作者 4.出版社：";
        cin >> way;
        cout << "请输入查询关键词：";
        cin >> key;
        bool found = false;
        for (int i = 0; i < num; ++i)
        {
            if (booklist[i] == nullptr) continue;
            bool match = false;
            switch (way)
            {
            case 1:match = (booklist[i]->getBookname() == key); break;
            case 2:match = (booklist[i]->getISBN() == key); break;
            case 3:match = (booklist[i]->getAuthor() == key); break;
            case 4:match = (booklist[i]->getPh() == key); break;
            default:cout << "无效选项！" << endl; return;
            }
            if (match)
            {
                cout << "\n==================== 查询到图书 ====================" << endl;
                booklist[i]->show(getTotalByname(booklist[i]->getBookname()));
                found = true;
            }
        }
        if (!found)
        {
            cout << "无相符合的匹配内容！" << endl;
        }
    }
    //2.3修改功能
    void modify()
    {
        string isbn;
        cout << "请输入要修改书籍的ISBN码：";
        cin >> isbn;
        int idx = findbookbyISBN(isbn);
        if (idx == -1 || booklist[idx] == nullptr)
        {
            cout << "该ISBN码不存在！";
            return;
        }
        string newname, newph, newauthor;
        double newprice;
        cout << "请输入新书名："; cin >> newname;
        cout << "输入新出版社名："; cin >> newph;
        cout << "输入新作者名："; cin >> newauthor;
        cout << "输入新价格："; cin >> newprice;
        booklist[idx]->setBookname(newname);
        booklist[idx]->setPh(newph);
        booklist[idx]->setAuthor(newauthor);
        booklist[idx]->setPrice(newprice);
        cout << "修改成功！";
    }
    //2.3删除功能
    void remove()
    {
        string isbn;
        cout << "请输入要删除目标书籍的ISBN码：";
        cin >> isbn;
        int idx = findbookbyISBN(isbn);
        if (idx == -1 || booklist[idx] == nullptr)
        {
            cout << "该ISBN码不存在！";
            return;
        }
        delete booklist[idx];
        for (int i = idx; i < num - 1; ++i)
        {
            booklist[i] = booklist[i + 1];
        }
        num--;
        booklist[num] = nullptr; // 删除后置空最后一个指针
        cout << "删除成功！" << endl;
    }
    //3.统计功能
    void statistics()
    {
        cout << "\n==================== 图书统计 ====================" << endl;
        cout << "当前图书总量：" << num << endl;
        cout << "所有图书详细信息：" << endl;
        for (auto book:booklist)
        {
            if (book != nullptr)
                book->show(getTotalByname(book->getBookname()));
        }
        int way;
        cout << "请选择统计排序的方式：1.价格 2.库存量 3.作者 4.出版社：";
        cin >> way;
        vector<Book*> sortedlist;
        // 只将有效书籍加入排序列表
        //不动原本Booklist数组中的顺序 相当于拷贝了Booklist的值到sortedlist数组来
        for (auto book : booklist)
        {
            if (book != nullptr)
                sortedlist.push_back(book);
        }
        //Lambda表达式：[捕获列表](参数类型 参数名){比较逻辑}
        switch (way)
        {
        case 1: sort(sortedlist.begin(), sortedlist.end(), [](Book* a, Book* b) {return a->getPrice() > b->getPrice(); });//价格从高到低
            break;
        case 2: sort(sortedlist.begin(), sortedlist.end(), [this](Book* a, Book* b) { return getTotalByname(a->getBookname()) > getTotalByname(b->getBookname()); });//库存量从高到低
            break;
        case 3: sort(sortedlist.begin(), sortedlist.end(), [](Book* a, Book* b) {return a->getAuthor() > b->getAuthor(); });//作者字典从高到低
            break;
        case 4: sort(sortedlist.begin(), sortedlist.end(), [](Book* a, Book* b) {return a->getPh() > b->getPh(); });//出版社字典从高到低
            break;
        default:
            cout << "无效选项！" << endl;
            return;
        }

        cout << "\n==================== 排序后结果 ====================" << endl;
        for (auto b : sortedlist)
        {
            if (b != nullptr)
                b->show(getTotalByname(b->getBookname()));
        }
    }
    //4.图书存盘
    void saveToFile(const string& filename)
    {
        ofstream ofs(filename);
        if (!ofs)
        {
            cout << "文件打开失败！";
            return;
        }
        ofs << num << endl;
        for (int i = 0; i < num; i++)
        {
            if (booklist[i] == nullptr) continue;
            ofs << booklist[i]->getBookname() << " "
                << booklist[i]->getPh() << " "
                << booklist[i]->getISBN() << " "
                << booklist[i]->getAuthor() << " "
                << booklist[i]->getPrice() << endl;
        }
        ofs.close();
        cout << "图书信息已成功保存到文件！";
    }
    //5.读出文件信息
    void loadFromFile(const string& filename)
    {
        ifstream ifs(filename);
        if (!ifs)
        {
            cout << "文件打开失败！" << endl;
            return;
        }
        // 清空旧数据并置空所有指针
        for (int i = 0; i < num; i++)
        {
            if (booklist[i] != nullptr)
            {
                delete booklist[i];
                booklist[i] = nullptr;
            }
        }
        num = 0;
        int filenum;
        ifs >> filenum;
        ifs.ignore();
        //检测是否是因为文本格式多空格导致的显示问题
        for (int i = 0; i < filenum; i++)
        {
            string bn, ph, isbn, a;
            double p;
            if (!(ifs >> bn >> ph >> isbn >> a >> p))
            {
                cout << " ！！！第 " << i + 1 << " 行数据格式错误！请检查是否少字段/多空格/隐藏字符。" << endl;
                return;
            }
            // 校验价格合法性，避免读到坏数据
            if (p <= 0)
            {
                cout << " ！！！第 " << i + 1 << " 行价格异常（当前值：" << p << "），可能是换行/空格导致字段错位！" << endl;
                return;
            } 
            //读取数据
            booklist[num++] = new Book(bn, ph, isbn, a, p);
        }
        // 将超出文件数量的指针全部置空，防止野指针
        for (int i = filenum; i < 10000; i++)
            booklist[i] = nullptr;
        ifs.close();
        cout << " 成功读取 " << filenum << " 本图书！" << endl;
    }
    //6.菜单系统
    void showMenu()
    {
        int c;
        while (true)
        {
            cout << "\n=============================================\n";
            cout << "||              图书管理系统                 ||\n";
            cout << "=============================================\n";
            cout << "||  1. 销售图书    |   2. 添加图书           ||\n";
            cout << "||  3. 查询图书    |   4. 修改图书           ||\n";
            cout << "||  5. 删除图书    |   6. 统计排序           ||\n";
            cout << "||  7. 图书存盘    |   8. 读出文件信息       ||\n";
            cout << "||  0. 退出系统    |                         ||\n";
            cout << "=============================================\n";
            cout << "  使用提示：打开程序请先按【8 读取】，再操作图书，最后按【7 保存】！\n";
            cout << "=============================================\n";
            cout << "请输入操作选项：";
            cin >> c;

            if (c == 1) shop();
            else if (c == 2) add();
            else if (c == 3) query();
            else if (c == 4) modify();
            else if (c == 5) remove();
            else if (c == 6) statistics();
            else if (c == 7) saveToFile("books.txt");
            else if (c == 8) loadFromFile("books.txt");
            else if (c == 0) {
                cout << "\n=============================================\n";
                cout << "||              感谢使用，再见！            ||\n";
                cout << "=============================================\n";
                break;
            }
            else {
                cout << "\n 无效输入，请重新选择！\n";
                cin.clear();
                cin.ignore(1024, '\n');
            }
        }
    }
};

int main()
{
    Function sys;
    sys.showMenu();
    return 0;
}