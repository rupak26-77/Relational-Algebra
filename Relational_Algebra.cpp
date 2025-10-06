#include <bits/stdc++.h>
using namespace std;
#include <stdexcept>
map<string, int> preced;
class relation
{
public:
    string table_name = "temp";
    set<vector<string>> data;
    vector<string> attributes;
    vector<string> types;
    map<string, int> indexes; 
    relation(string s1, vector<string> v1, vector<string> v2)
    {
        this->table_name = s1;
        this->attributes = v1;
        this->types = v2;
        for (int i = 0; i < attributes.size(); i++)
        {
            indexes[attributes[i]] = i;
        }
    }
    relation(vector<string> v1, vector<string> v2)
    {
        this->attributes = v1;
        this->types = v2;
        for (int i = 0; i < attributes.size(); i++)
        {
            indexes[attributes[i]] = i; // mapping 
        }
    }
    void display()
    {
        for (auto it : this->attributes)
        {
            printf("%-15s ", it.c_str());
            printf(" | ");
        }
        cout << "\n";
        for (int i = 0; i <= 18 * attributes.size() + 2; i++) cout << "-";
        //each column has 15 chars + " | " (~3 extra chars)
        cout << "\n";
        for (auto it : this->data)
        {
            for (auto it2 : it)
            {
                printf("%-15s ", it2.c_str());
                printf(" | ");
            }
            printf("\n");
        }
    }
    /*
        id              | name            | 
        -------------------------------------
        1               | Alice           | 
        2               | Bob             | 
    */
};

//table name -> table pointer
map<string, relation *> table_pointer ;

relation *cross_prod(relation *r1, relation *r2)
{
    if (r1 == NULL || r2 == NULL) return NULL;
    string s1 = r1->table_name + 'X' + r2->table_name;
    vector<string> types = r1->types;
    for (auto it : r2->types) types.push_back(it);
    vector<string> attributes;
    for (auto it : r1->attributes)
    {
        if (r2->indexes.find(it) != r2->indexes.end())
        {
            attributes.push_back(r1->table_name + "." + it);
        }
        else attributes.push_back(it);
    }
    for (auto it : r2->attributes)
    {
        if (r1->indexes.find(it) != r1->indexes.end())
        {
            attributes.push_back(r2->table_name + "." + it);
        }
        else
            attributes.push_back(it);
    }
    relation *r = new relation(s1, attributes, types);
    for (auto it : r1->data)
    {
        vector<string> v = it;
        for (auto it2 : r2->data)
        {
            for (auto it3 : it2) v.push_back(it3);
            r->data.insert(v);
            while (v.size() != it.size())
                v.pop_back();
        }
    }
    return r;
}
int compa(relation *r1 , relation *r2)
{
    if (r1 == NULL || r2 == NULL)
        return 0 ;
    if (r1->attributes.size() != r2->attributes.size())
        return 0;
    for (int i = 0; i < r1->types.size(); i++)
    {
        if (r1->types[i] != r2->types[i])
            return 0;
    }
    return 1 ;
    
}
relation *unioon(relation *r1, relation *r2)
{
    if (r1 == NULL || r2 == NULL)
        return 0;
    string name = r1->table_name;
    int compati = compa(r1, r2);
    if (!compati)
    {
        cout << "THE ABOVE MENTIONED relationS AREN'T UNION COMPATIBLE";
        return NULL;
    }
    relation *r = new relation(name, r1->attributes, r1->types);
    for (auto it : r1->data)
    {
        r->data.insert(it);
    }
    for (auto it : r2->data)
    {
        if (r1->data.find(it) == r1->data.end())
            r->data.insert(it);
    }
    return r;
}
relation *difference(relation *r1, relation *r2)
{
    if (r1 == NULL || r2 == NULL)
        return 0;
    string name = r1->table_name;
    int compati = compa(r1, r2);
    if (!compati)
    {
        cout << "THE ABOVE MENTIONED relationS AREN'T UNION COMPATIBLE";
        return NULL;
    }
    relation *r = new relation(name, r1->attributes, r1->types);
    for (auto it : r1->data)
    {
        if (r2->data.find(it) == r2->data.end())
        {
            r->data.insert(it);
        }
    }
    return r;
}
relation *project(relation &r1, vector<string> attr)
{
    int flag = 0;
    vector<int> indices;
    for (auto it : attr)
    {
        if (r1.indexes.find(it) == r1.indexes.end())
        {
            flag = 1;
            break;
        }
        indices.push_back(r1.indexes[it]);
    }
    if (flag)
    {
        cout << "THE ABOVE ENTERED SET OF ATTRIBUTES DOESN'T BELONG TO THE relation\n";
        return NULL;
    }
    vector<string> types;
    for (auto it : attr)
    {
        types.push_back(r1.types[r1.indexes[it]]);
    }
    relation *r = new relation(r1.table_name, attr, types);
    /*
        r1.data = { ["1","Rupak","IT"], ["2","Sidharth","HR"], ["3","Ankit","HR"] }
        attr = ["name","dept"]
        indices = [1,2]
     */
    for (auto it : r1.data)
    {
        vector<string> temp_data;
        for (auto it2 : indices)
            temp_data.push_back(it[it2]);
        r->data.insert(temp_data);
    }
    return r;
}

int evaluate(string exp, vector<string> values, map<string, int> indices)
{
    if (exp.size() == 0)
        return 1;
    stack<string> s2;
    int val;
    for (int i = 0; i < exp.length(); i++)
    {
        if (exp[i] == ' ')continue;
        string s4;
        int j = i;
        for (;; j++)
        {
            if (exp[j] == ' ')
                break;
            s4 += exp[j];
        }
        i = j;
        if (s4[0] == ']' || s4[0] == '[' || s4[0] == ';' || s4[0] == ',' || s4[0] == '#' || s4 == "==")
        {
            return -1;
        }
        if (s4[0] == '\'') 
        {
            s2.push(s4.substr(1, s4.length() - 2));
            continue;
        }
        // Check if it's an operator
        else if ((s4[0] < 'a' || s4[0] > 'z') & (s4[0] < 'A' || s4[0] > 'Z') & (s4[0] < '0' || s4[0] > '9') & (s4[0] != '\''))
        {
            string y = s2.top();
            s2.pop();
            string x = s2.top();
            s2.pop();
            int flag = 0;   //0 for numbers, 1 for strings/attributes
            if (indices.find(x) == indices.end())
            {   // checking if x is a column name or numeric number
                for (int i = 0; i < x.length(); i++)
                {
                    if ((x[i] >= 'a' && x[i] <= 'z') || (x[i] >= 'A' && x[i] <= 'Z'))
                        flag = 1;
                    if (x[i] == '.')
                        flag = 1;
                }
            }
            if (indices.find(y) == indices.end())
            {
                for (int i = 0; i < y.length(); i++)
                {
                    if ((y[i] >= 'a' && y[i] <= 'z') || (y[i] >= 'A' && y[i] <= 'Z'))
                        flag = 1;
                    if (y[i] == '.')
                        flag = 1;
                }
            }
            if (flag)
            {
                if (preced.find(s4) == preced.end())
                    return -1;
                if (s4 == "<=")
                {
                    s2.push(to_string(x <= y));
                }
                else if (s4 == "<")
                {
                    s2.push(to_string(x < y));
                }
                else if (s4 == ">")
                {
                    s2.push(to_string(x > y));
                }
                else if (s4 == ">=")
                {
                    s2.push(to_string(x >= y));
                }
                if (s4 == "=")
                {
                    s2.push(to_string(x == y));
                }
                else if (s4 == "!")
                {
                    s2.push(to_string(x != y));
                }
            }
            // Integer arithmetic/comparison
            else
            {
                // int x1 = 1 , y1=2;
                for (int i = 0; i < x.length(); i++)
                {
                    if (x[i] > '9' | x[i] < '0')
                        return -1;
                }
                for (int i = 0; i < y.length(); i++)
                {
                    if (y[i] > '9' | y[i] < '0')
                        return -1;
                }
                int x1 = stoi(x), y1 = stoi(y);
                if (s4 == "<=")
                {
                    s2.push(to_string(x1 <= y1));
                }
                else if (s4 == "<")
                {
                    // cout<<x<<" "<<y<<" "<<(x<y)<<"\n";
                    s2.push(to_string(x1 < y1));
                }
                else if (s4 == ">")
                {
                    // cout<<x<<" "<<y<<" "<<(x>y)<<"\n";
                    s2.push(to_string(x1 > y1));
                }
                else if (s4 == ">=")
                {
                    s2.push(to_string(x1 >= y1));
                }
                if (s4 == "=")
                {
                    s2.push(to_string(x1 == y1));
                }
                else if (s4 == "!")
                {
                    s2.push(to_string(x1 != y1));
                }
                else if (s4 == "&")
                {
                    // cout<<x<<" "<<y<<" "<<(x&y)<<"\n";
                    s2.push(to_string(x1 & y1));
                }
                else if (s4 == "|")
                {
                    // cout<<x<<" "<<y<<" "<<(x|y)<<"\n";
                    s2.push(to_string(x1 | y1));
                }
                else if (s4 == "^")
                {
                    s2.push(to_string(x1 ^ y1));
                }
                else if (s4 == "+")
                {
                    s2.push(to_string(x1 + y1));
                }
                else if (s4 == "-")
                {
                    s2.push(to_string(x1 - y1));
                }
                else if (s4 == "/")
                {
                    s2.push(to_string(x1 / y1));
                }
                else if (s4 == "*")
                {
                    s2.push(to_string(x1 * y1));
                }
            }
        }
        //s4 is Operand (attribute name or numeric literal)
        else
        {
            int flag = 0;
            for (auto it : s4)
            {
                if (it < '0' | it > '9')
                    flag = 1;
                if (it == '.')
                    flag = 2;
            }
            if (flag == 1)
            {
                if (indices.find(s4) != indices.end())
                    s2.push((values[indices[s4]]));
                else
                {
                    return -1; // Invalid attribute name
                }
            }
            else
            {
                s2.push(s4);
            }
        }
    }
    // After processing all tokens, the stack should contain the final result
    int flag = 0;
    for (auto it : s2.top())
    {
        if (it > '9' | it < '0')
            flag = -1;
    }
    if (flag == -1)
        return flag;
    return stoi(s2.top());
}
relation *seelect(string exp, relation &r)
{
    relation *r1 = new relation(r.table_name, r.attributes, r.types);
    for (auto &it : r.data)
    {
        int x = evaluate(exp, it, r.indexes);
        if (x == -1)
        {
            // cout<<"ENTER A VALID PREDICATE \n";
            return NULL;
        }
        if (evaluate(exp, it, r.indexes))
            r1->data.insert(it);
    }
    return r1;
}

string postfix(string s)
{
    int n = s.length();
    stack<string> s2;
    string s3;
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == ' ')
            continue;
        string s4;
        s4 += s[i];
        int j = i + 1;
        for (; j < n; j++)
        {
            if (s[j] == ' ')
                break;
            else
                s4 += s[j];
        }
        i = j;
        if (s4[0] == '\'')
        {
            s3 += s4;
        }
        else if (s4[0] == '(')
        {
            s2.push(s4);
        }
        else if (s4 == ")")
        {
            while (s2.top() != "(")
            {
                s3 += s2.top();
                s3 += " ";
                s2.pop();
            }
            s2.pop();
        }
        else if ((s4[0] < 'a' || s4[0] > 'z') & (s4[0] < 'A' || s4[0] > 'Z') & (s4[0] < '0' || s4[0] > '9'))
        {
            if (s2.empty())
            {
                s2.push(s4);
            }
            else
            {
                while (preced[s2.top()] > preced[s4])
                {
                    s3 += s2.top();
                    s3 += " ";
                    s2.pop();
                    if (s2.empty())
                        break;
                }
                s2.push(s4);
            }
        }
        else
            s3 += s4;
        s3 += " ";
    }
    while (!s2.empty())
    {
        s3 += s2.top();
        s3 += " ";
        s2.pop();
    }
    return s3;
}

relation *paarse(string querry)
{
    int n = querry.length();
    int y = 0;
    for (; y < n; y++)
    {
        if (querry[y] != ' ')
        {
            break;
        }
    }
    querry = querry.substr(y, n - y);
    if (querry[0] == 'S')
    {
        // S [ pnum > 3 & dnum < 3 ] { proj }
        int start = 0, fin = 0;
        int pred_s = -1, pred_e;
        int rel_s = 0, rel_f = 0;
        for (int i = 0; i < n; i++)
        {
            if (querry[i] == '[' & pred_s == -1)
            {
                pred_s = i;
            }
            if (querry[i] == ']')
            {
                pred_e = i;
                break;
            }
        }
        for (; start < querry.length(); start++)
        {
            if (querry[start] == '{')
            {
                rel_s = start;
                start++;
                break;
            }
            if (querry[start] != ']' && querry[start] != ' ')
            {
                fin = start;
            }
        }
        for (; start < querry.length(); start++)
        {
            if (querry[start] == '}')
            {
                rel_f = start;
            }
        }
        string pred = postfix(querry.substr(pred_s + 1, pred_e - 1 - pred_s));
        relation *r = paarse(querry.substr(rel_s + 1, rel_f - 1 - rel_s));
        if (r == NULL)
        {
            return NULL;
        }
        return seelect(pred, *r);
    }
    // P[attribute1 attribute2 ...]{relation_expression}
    else if (querry[0] == 'P')
    {
        int start = 0;
        for (int i = 0; i < querry.length(); i++)
        {
            if (querry[i] == '{')
            {
                start = i;
                break;
            }
        }
        vector<string> attr;
        for (int i = 0; i < n; i++)
        {
            if (querry[i] == '[')
            {
                string s1;
                for (int j = i + 1; querry[j] != ']'; j++)
                {
                    if (querry[j] == ' ')
                    {
                        if (s1.size())
                        {
                            attr.push_back(s1);
                            s1 = "";
                        }
                        continue;
                    }
                    s1 += querry[j];
                }
                break;
            }
        }
        int end = querry.length() - 1;
        while (querry[end] != '}')
        {
            end--;
        }
        relation *r = paarse(querry.substr(start + 1, end - start - 1));
        if (r == NULL)
        {
            return NULL;
        }
        return project(*r, attr);
    }
    else if (querry[0] == 'U')
    {
        string sr1, sr2;
        int count = 1;
        int i = 0;
        for (; i < n; i++)
        {
            if (querry[i] == '{')
            {
                i++;
                break;
            }
        }
        for (; i < n; i++)
        {
            if (querry[i] == '{')
            {
                count++;
            }
            else if (querry[i] == '}')
            {
                count--;
                if (count == 0)
                    break;
            }
            sr1 += querry[i];
        }
        for (; i < n; i++)
        {
            if (querry[i] == '{')
            {
                i++;
                break;
            }
        }
        count = 1;
        for (; i < n; i++)
        {
            if (querry[i] == '{')
            {
                count++;
            }
            else if (querry[i] == '}')
            {
                count--;
                if (count == 0)
                    break;
            }
            sr2 += querry[i];
        }
        relation *r1 = paarse(sr1), *r2 = paarse(sr2);
        return unioon(r1, r2);
    }
    else if (querry[0] == 'D')
    {
        string sr1, sr2;
        int count = 1;
        int i = 0;
        for (; i < n; i++)
        {
            if (querry[i] == '{')
            {
                i++;
                break;
            }
        }
        for (; i < n; i++)
        {
            if (querry[i] == '{')
            {
                count++;
            }
            else if (querry[i] == '}')
            {
                count--;
                if (count == 0)
                    break;
            }
            sr1 += querry[i];
        }
        for (; i < n; i++)
        {
            if (querry[i] == '{')
            {
                i++;
                break;
            }
        }
        count = 1;
        for (; i < n; i++)
        {
            if (querry[i] == '{')
            {
                count++;
            }
            else if (querry[i] == '}')
            {
                count--;
                if (count == 0)
                    break;
            }
            sr2 += querry[i];
        }
        relation *r1 = paarse(sr1), *r2 = paarse(sr2);
        if (r1 == NULL || r2 == NULL)
        {
            return NULL;
        }
        return difference(r1, r2);
    }
    else if (querry[0] == 'C')
    {
        string sr1, sr2;
        int count = 1;
        int i = 0;
        for (; i < n; i++)
        {
            if (querry[i] == '{')
            {
                i++;
                break;
            }
        }
        for (; i < n; i++)
        {
            if (querry[i] == '{')
            {
                count++;
            }
            else if (querry[i] == '}')
            {
                count--;
                if (count == 0)
                    break;
            }
            sr1 += querry[i];
        }
        for (; i < n; i++)
        {
            if (querry[i] == '{')
            {
                i++;
                break;
            }
        }
        count = 1;
        for (; i < n; i++)
        {
            if (querry[i] == '{')
            {
                count++;
            }
            else if (querry[i] == '}')
            {
                count--;
                if (count == 0)
                    break;
            }
            sr2 += querry[i];
        }
        // cout<<sr1<<"\n"<<sr2<<"\n";
        // return nullptr;
        relation *r1 = paarse(sr1), *r2 = paarse(sr2);
        if (r1 == NULL || r2 == NULL)
        {
            return NULL;
        }
        return cross_prod(r1, r2);
    }
    else if (querry[0] == 'R')
    {
        int start1 = -1, end1 = -1, start2 = -1, end2 = -1;
        string newr;
        vector<string> newattr;
        for (int i = 0; i < n; i++)
        {
            if (querry[i] == '[')
            {
                start1 = i + 1;
                continue;
            }
            if (querry[i] == ']')
            {
                end1 = i + 1;
                break;
            }
            if (start1 != -1)
            {
                if (querry[i] != ' ')
                {
                    newr += querry[i];
                }
            }
        }
        for (int i = end1; i < n; i++)
        {
            if (querry[i] == '[')
            {
                start2 = i + 1;
                continue;
            }
            if (start2 != -1)
            {
                if (querry[i] == ']')
                {
                    end2 = i + 1;
                    break;
                }
                if (querry[i] != ' ')
                {
                    int j = i;
                    string s2;
                    for (; j < n; j++)
                    {
                        if (querry[j] == ' ')
                        {
                            newattr.push_back(s2);
                            i = j;
                            break;
                        }
                        s2 += querry[j];
                    }
                }
            }
        }
        string r;
        int start3 = -1, end3 = -1;
        for (int i = end2; i < n; i++)
        {
            if (querry[i] == '{' & start3 == -1)
            {
                start3 = i;
            }
            else if (querry[i] == '}')
            {
                end3 = i;
            }
        }
        relation *rb = paarse(querry.substr(start3 + 1, end3 - 1 - start3));
        if (rb == NULL)
            return NULL;
        if (newattr.size() != rb->types.size())
            return NULL;
        relation *ra = new relation(newr, newattr, rb->types);
        for (auto it : rb->data)
        {
            ra->data.insert(it);
        }
        return ra; 
    }
    else
    {
        string s;
        for (auto it : querry)
        {
            if (it == ' ')
                continue;
            s += it;
        }
        if (table_pointer.find(s) == table_pointer.end())
            return NULL;
        return table_pointer[s];
    }
}

int main()
{
    preced["|"] = 1;
    preced["^"] = 2;
    preced["&"] = 3;
    preced["="] = 4;
    preced["!"] = 4;
    preced["<"] = 5;
    preced["<="] = 5;
    preced[">"] = 5;
    preced[">="] = 5;
    preced["+"] = 6;
    preced["-"] = 6;
    preced["/"] = 7;
    preced["*"] = 7;
    // defining attribute name and datatypes :
    vector<string> attr5 = {"fname", "ssn"};
    vector<string> types5 = {"string", "string"};
    vector<string> attr6 = {"dno", "dloc"};
    vector<string> types6 = {"int", "string"};
    vector<string> attr7 = {"pnum", "dnum"};
    vector<string> types7 = {"int", "int"};
    vector<string> attr8 = {"essn", "pno"};
    vector<string> types8 = {"string", "int"};
    relation *employee = new relation("employee", attr5, types5);
    relation *dept = new relation("dept", attr6, types6);
    relation *proj = new relation("proj", attr7, types7);
    relation *works = new relation("works", attr8, types8);
    // entering data into employee table :
    employee->data.insert({"a", "1"});
    employee->data.insert({"b", "2"});
    employee->data.insert({"c", "3"});
    employee->data.insert({"d", "4"});
    employee->data.insert({"e", "5"});
    // entering data into dept table :
    dept->data.insert({"1", "tosham"});
    dept->data.insert({"1", "dlhi"});
    dept->data.insert({"2", "hssr"});
    dept->data.insert({"2", "bhw"});
    dept->data.insert({"3", "rhtk"});
    // entering data into proj table :
    proj->data.insert({"1", "1"});
    proj->data.insert({"2", "1"});
    proj->data.insert({"3", "2"});
    proj->data.insert({"4", "2"});
    proj->data.insert({"5", "3"});
    // entering data into works table :
    works->data.insert({"1", "1"});
    works->data.insert({"1", "2"});
    works->data.insert({"1", "3"});
    works->data.insert({"2", "2"});
    works->data.insert({"2", "3"});
    works->data.insert({"3", "4"});
    works->data.insert({"3", "3"});
    works->data.insert({"3", "1"});
    works->data.insert({"4", "5"});
    works->data.insert({"1", "5"});
    table_pointer["dept"] = dept;
    table_pointer["proj"] = proj;
    table_pointer["works"] = works;
    table_pointer["employee"] = employee;
    cout << "ALL TABLES\n";
    cout << "\temployee\n";
    employee->display();
    cout << "\n";
    cout << "\tworks\n";
    works->display();
    cout << "\n";
    cout << "\tproj\n";
    proj->display();
    cout << "\n";
    cout << "\tdept\n";
    dept->display();
    cout << "\n";
    cout << "RESULTS\n\n";
    // QUERY-1 Find the employees (name and ssn) who work on all the projects affiliated with a department with at least two locations.
    string querry2 = "P [ fname essn ] { S [ essn = ssn ] { C { P [ essn ] { D { P [ essn dno ] { P [ essn pnum dno ] { S [ pnum = pno ] { C { P [ dno pnum ] { S [ dno = dnum ] { C { P [ dno ] { S [ dno2 = dno & dloc2 ! dloc ] { C { R [ dept2 ] [ dno2 dloc2 ] { dept } } { dept } } } } { proj } } } } { works } } } } } { P [ essn dno ] { D { C { P [ essn ] { P [ essn pnum dno ] { S [ pnum = pno ] { C { P [ dno pnum ] { S [ dno = dnum ] { C { P [ dno ] { S [ dno2 = dno & dloc2 ! dloc ] { C { R [ dept2 ] [ dno2 dloc2 ] { dept } } { dept } } } } { proj } } } } { works } } } } } { P [ pnum dno ] { P [ essn pnum dno ] { S [ pnum = pno ] { C { P [ dno pnum ] { S [ dno = dnum ] { C { P [ dno ] { S [ dno2 = dno & dloc2 ! dloc ] { C { R [ dept2 ] [ dno2 dloc2 ] { dept } } { dept } } } } { proj } } } } { works } } } } } } { P [ essn pnum dno ] { S [ pnum = pno ] { C { P [ dno pnum ] { S [ dno = dnum ] { C { P [ dno ] { S [ dno2 = dno & dloc2 ! dloc ] { C { R [ dept2 ] [ dno2 dloc2 ] { dept } } { dept } } } } { proj } } } } { works } } } } } } } } { employee } } }";
    relation *r2 = paarse(querry2);
    cout << "QUERY-1\n";
    if (r2 != NULL)
        r2->display();
    else
        cout << "ENTER A VALID EXPRESSION\n";
    cout << "\n\n";

    // QUERY-2 Select all the employees whose project number is greater than 3 and department number is less than 3.
    string querry3 = "S [ pnum > 3 &  dnum < 3 ] { proj }";
    relation *r3 = paarse(querry3);
    cout << "QUERY-2\n";
    if (r3 != NULL)
        r3->display();
    else
        cout << "ENTER A VALID EXPRESSION\n";
    cout << "\n\n";

    // QUERY-3 Same as Query-2 but writing wrong RA ro handle errors
    cout << "QUERY-3\n";
    string querry31 = "S [ pnum_wrong_attribute > 3 &  dnum < 3 ] { proj }";
    relation *r31 = paarse(querry31);
    if (r31 != NULL)
        r31->display();
    else
        cout << "ENTER A VALID EXPRESSION\n";
    cout << "\n\n";

    // QUERY-4 Write a query to display the name , ssn of all the employees who are working on more than one project
    string querry4 = "R [ works2 ] [ essn2 pno2 ] { works }";
    relation *works2 = paarse(querry4);
    table_pointer["works2"] = works2;

    string querry5 = "P [ essn2 ] { S [ essn2 = essn & pno2 ! pno ] { C { works2 } { works } } }";
    relation *r6 = paarse(querry5);
    table_pointer["r6"] = r6;
    cout << "QUERY-4\n";
    string querry7 = "P [ ssn fname ]  { S [ ssn = essn2 ] { C { employee } { r6 } } }";
    // P [ ssn fname ]  { S [ ssn = essn2 ] { C { employee } { P [ essn2 ] { S [ essn2 = essn & pno2 ! pno ] { C { R [ works2 ] [ essn2 pno2 ] { works } } { works } } } } } }
    relation *r7 = paarse(querry7);
    if (r7 != NULL)
        r7->display();
    else
        cout << "ENTER A VALID EXPRESSION\n";
    vector<string> a8 = {"a", "b"};
    vector<string> t8 = {"string", "string"};
    vector<string> a9 = {"a", "b", "c", "d"};
    vector<string> t9 = {"string", "string", "string", "string"};
    relation *r8 = new relation("r8", a8, t8);
    relation *r9 = new relation("r9", a9, t9);
    cout << "\n\n\n";
    table_pointer["r8"] = r8;
    table_pointer["r9"] = r9;
    r8->data.insert({"a1", "b1"});
    r8->data.insert({"a2", "b2"});
    r9->data.insert({"a1", "b1", "c1", "d1"});
    r9->data.insert({"a1", "b1", "c2", "d2"});
    r9->data.insert({"a1", "b1", "c3", "d3"});
    r9->data.insert({"a2", "b2", "c1", "d1"});
    r9->data.insert({"a2", "b2", "c3", "d3"});
    string querry8 = "D { P [ c d ] { r9 } } { P [ c d ] { r9 } } } { r9 } { P [ c d ] { D { C { r8 } } }";
    relation *r10 = paarse(querry8);
    r10->display();
    cout << "\n\n\n";
    string querry10 = "S [ 1 ] { proj }";
    relation *r12 = paarse(querry10);
    r12->display();
    cout << "\n\n\n";
    string querry11 = "S [ dloc = 'dlhi' ] { P [ pnum dnum dloc ] { S [ dnum = dno ] { C { proj } {dept} } } } ";
    relation *r14 = paarse(querry11);
    r14->display();
    return 0;
}