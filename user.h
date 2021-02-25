//
// Created by 16963 on 2021/2/20.
//

#ifndef UNTITLED5_USER_H
#define UNTITLED5_USER_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <stack>
#include<unordered_map>
#include "bookstore.h"
#include "handle.h"
#include<iomanip>
#include<algorithm>
using namespace std;
#define cd(x)  (char*)&x
const string aofu="allusers.txt";
const string aofb="allbooks.txt";
const string ISBNfile="ISBNfile.txt";
const string authorfile="authorfile.txt";
const string userfile="userfile.txt";
const string keywordfile="keywordfile.txt";
const string namefile="namefile.txt";
const string ISBNpool="ISBNpool.txt";
const string authorpool="authorpool.txt";
const string userpool="userpool.txt";
const string keywordpool="keywordpool.txt";
const string namepool="namepool.txt";
const string finance="finance.txt";
handle operating;
double stringtodouble(string str) {
    int pos=str.find('.');
    if(pos==str.npos){
        pos=str.length();
    }
    double x=0;
    double y=0;
    int l=str.length();
    for(int i=0;i<pos;i++) x=x*10+(str[i]-'0');
    for(int i=l-1;i>=pos+1;i--) {
        y=y*0.1+(str[i]-'0');
    }
    y*=0.1;
    return (x+y);
}
void showuser(user& u){
    cout<<u.userid<<'\t'<<u.passwd<<'\t'<<u.name<<'\t'<<u.priviledge<<endl;
}
void showbook(book& bo){
    cout<<bo.IBSN<<'\t'<<bo.name<<'\t'<<bo.author<<'\t'<<bo.keyword<<'\t'<<fixed<<setprecision(2)<<bo.price<<'\t'<<bo.quantity<<endl;
}
bool ISBNsort(book& b1,book& b2){
    if(strcmp(b1.IBSN,b2.IBSN)<=0) return 1;
    else return 0;
}
void showalluser() {
    fstream file;
    file.open(aofu, ios::in |ios::out| ios::binary);
    int num;
    file.read(cd(num), sizeof(int));
    user tmp;
    for (int i = 0; i < num; i++) {
        file.read(cd(tmp), sizeof(user));
        showuser(tmp);
    }
    file.close();
}
void showallbook(){
    fstream file;
    file.open(aofb,ios::in|ios::out|ios::binary);
    int num;
    file.read(cd(num), sizeof(int));
    book tmp;
    for (int i = 0; i < num; i++) {
        file.read(cd(tmp), sizeof(book));
        showbook(tmp);
    }
    file.close();
}
class Zero{
private:
    const int priviledge=0;
protected:
    const int i=sizeof(int);
    const int b=sizeof(block);
    const int n=sizeof(node);
    const int u=sizeof(user);
    const int bo=sizeof(book);
public:
    Zero()=default;
    ~Zero(){}
    bool su(queue<string>* todos){
     //   char* passwd;
        int size=todos->size();
        fstream file;
        file.open(aofu,ios::in|ios::out|ios::binary);
        user tmp;
        if(size==1) {
            string userid=todos->front();
            int *poss;
            try {
                poss = users.find(userid.c_str());
               // cout<<"findok"<<endl;
            }catch(NotFound){delete todos;file.close();throw Error();}
            int pos=poss[1];
           // user tmp;
            file.seekg(i+(pos-1)*u);
            file.read(cd(tmp),u);
            //压入operating中
            delete[]poss;
            //operating.login(tmp);
        }
        else if(size==2) {
            string userid=todos->front();
            int* poss;
            try {
                poss = users.find(userid.c_str());
            //    cout<<"findok"<<endl;
            }catch(NotFound){delete todos;file.close(); throw Error();}
            int pos=poss[1];
         //   user tmp;
            file.seekg(i+(pos-1)*u);
            file.read(cd(tmp),u);
            todos->pop();
            string passwd=todos->front();
           // cout<<passwd<<endl;
           // cout<<tmp.passwd<<endl;
            if(strcmp(passwd.c_str(),tmp.passwd)!=0){//密码不匹配
                delete poss;
                delete todos;
                file.close();
                throw Error();
            }
            delete[]poss;
        }
        //file.seekg(i+(pos[1]-1)*sizeof(user));
        //file.read(c(tmp),sizeof(user));
        operating.login(tmp);
     //   cout<<operating.getpri()<<endl;
        file.close();
    }
    void reg(queue<string>* todos){
        fstream file;
        file.open(aofu,ios::in|ios::out|ios::binary);
        string userid=todos->front();
        todos->pop();
        int *poss;
      //  cout<<"point 7"<<endl;
        try {
            poss = users.find(userid.c_str());
        }catch(NotFound){//找不到
            //在user文件顶部塞一个
        //    cout<<"point 8"<<endl;
            int pos=operating.newuser();
            user newu;
            newu.priviledge=1;
            string passwd=todos->front();
            todos->pop();
            string name=todos->front();
            todos->pop();
         //   cout<<"Not Found"<<endl;
           // newu.userid=new char[userid.length()];
            //newu.passwd=new char[passwd.length()];
            //newu.name=new char[name.length()];
            strcpy(newu.userid,userid.c_str());
            strcpy(newu.passwd,passwd.c_str());
            strcpy(newu.name,passwd.c_str());
           // cout<<newu.name<<endl;
            file.seekp(0);
            file.write(cd(pos),i);
            file.seekp(i+(pos-1)*u);
            file.write(cd(newu),u);
            users.insert(userid.c_str(),pos);
           // cout<<"point 9"<<endl;
          //  delete[] newu.userid;
          //  delete[] newu.name;
          //  delete[] newu.passwd;
         // users.showall();

            file.close();
            //delete[]poss;
            delete todos;
            return;
        }
        delete[]poss;
        delete todos;
        file.close();
    //    cout<<"Found"<<endl;
        throw Error();//已存在
    }
};
class Customer:public Zero{
private:
    const int priviledge=1;
protected:
    user infor;
public:
    Customer()=default;
    ~Customer(){}
    void repasswd(queue<string>* todos){
        fstream file;
        file.open(aofu,ios::in|ios::out|ios::binary);
        int size=todos->size();
        string userid=todos->front();
        todos->pop();
        int* poss;
        try{
            poss=users.find(userid.c_str());
        }catch(NotFound){delete todos;file.close();throw Error();}
        file.seekg(i+(poss[1]-1)*u);
        user tmp;
        file.read(cd(tmp),u);
        if(size==2){
            string newpasswd=todos->front();
            todos->pop();
            strcpy(tmp.passwd,newpasswd.c_str());
            file.seekp(i+(poss[1]-1)*u);
            file.write(cd(tmp),u);
        }
        else if(size==3){
            string oldpasswd=todos->front();
            todos->pop();
            string newpasswd=todos->front();
            todos->pop();
            if(strcmp(oldpasswd.c_str(),tmp.passwd)!=0)delete todos,file.close(), throw Error();
            strcpy(tmp.passwd,newpasswd.c_str());
            file.seekp(i+(poss[1]-1)*u);
            file.write(cd(tmp),u);
        }
        delete[]poss;
        file.close();
    }
    void show(queue<string>* todos){
        fstream file;
        file.open(aofb,ios::in|ios::out|ios::binary);
        string kind=todos->front();
        todos->pop();
        if(kind=="showall") {
            int* allpos;
            int all=operating.getab();
         //   ISBN.showall();
            try{
                allpos=ISBN.findall(all);
            }catch(NotFound){delete todos;cout<<'\n';
                return;}
            book temp;
            book* allbook=new book[all];
            allbook=new book[all];
            for(int j=0;j<all;j++){
                file.seekg(i+(allpos[j]-1)*bo);
                file.read(cd(temp),bo);
                allbook[j]=temp;
                //cout<<temp.IBSN<<' '<<temp.name<<' '<<temp.name<<' '<<temp.author<<' '<<temp.quantity<<' '<<temp.keyword<<' '<<fixed<<setprecision(2)<<temp.price;
            }
            sort(allbook,allbook+all,ISBNsort);
            for(int i=0;i<all;i++)
                showbook(allbook[i]);
            delete[] allpos;
            delete[] allbook;
        }
        else if(kind=="k") {
            string strkeyword=todos->front();
            todos->pop();
            int* pos;
            try{
                pos=keyword.find(strkeyword.c_str());
            }catch(NotFound) {
                delete todos;cout<<'\n';return;
            }
            book temp;
            book* allbook=new book[pos[0]];
          //  cout<<pos[0]<<"!"<<endl;
            for(int j=0;j<pos[0];j++) {
                file.seekg(i+(pos[j+1]-1)*bo);
                file.read(cd(temp),bo);
                allbook[j]=temp;
                //cout<<temp.IBSN<<' '<<temp.name<<' '<<temp.name<<' '<<temp.author<<' '<<temp.quantity<<' '<<temp.keyword<<' '<<fixed<<setprecision(2)<<temp.price;
            }
            sort(allbook,allbook+pos[0],ISBNsort);
            for(int i=0;i<pos[0];i++)  showbook(allbook[i]);
            delete[] pos;
            delete[] allbook;
        }
        else if(kind=="I"){
            string strISBN=todos->front();
            todos->pop();
            int* pos;
            try{
                pos=ISBN.find(strISBN.c_str());
            }catch(NotFound) {
                delete todos;cout<<'\n';return;
            }
            book temp;
            book* allbook=new book[pos[0]];
            for(int j=0;j<pos[0];j++) {
                file.seekg(i+(pos[j+1]-1)*bo);
                file.read(cd(temp),bo);
                allbook[j]=temp;
                //cout<<temp.IBSN<<' '<<temp.name<<' '<<temp.name<<' '<<temp.author<<' '<<temp.quantity<<' '<<temp.keyword<<' '<<fixed<<setprecision(2)<<temp.price;
            }
            sort(allbook,allbook+pos[0],ISBNsort);
            for(int i=0;i<pos[0];i++)  showbook(allbook[i]);
            delete[] pos;
            delete[] allbook;
        }
        else if(kind=="n"){
            string strname=todos->front();
            todos->pop();
            int* pos;
            try{
                pos=name.find(strname.c_str());
            }catch(NotFound) {
                delete todos;cout<<'\n';return;
            }
            book temp;
            book* allbook=new book[pos[0]];
            for(int j=0;j<pos[0];j++) {
                //cout<<pos[j+1]<<endl;
                file.seekg(i+(pos[j+1]-1)*bo);
                //cout<<file.tellg()<<endl;
                file.read(cd(temp),bo);
                allbook[j]=temp;
               // cout<<allbook[0].IBSN<<endl;
               // showbook(allbook[j]);
                //showbook(temp);
                //cout<<temp.IBSN<<' '<<temp.name<<' '<<temp.name<<' '<<temp.author<<' '<<temp.quantity<<' '<<temp.keyword<<' '<<fixed<<setprecision(2)<<temp.price;
            }
           // cout<<allbook[0].IBSN<<endl;
           // showbook(allbook[0]);
            sort(allbook,allbook+pos[0],ISBNsort);
          //  showbook(allbook[0]);
            for(int i=0;i<pos[0];i++)  showbook(allbook[i]);
            delete[] pos;
            delete[] allbook;
        }
        else if(kind=="a") {
            string strauthor=todos->front();
            todos->pop();
            int* pos;
            try{
                pos=author.find(strauthor.c_str());
            }catch(NotFound) {
               delete todos;cout<<'\n';return;
            }
            book temp;
            book* allbook=new book[pos[0]];
         //   cout<<pos[0]<<endl;
            for(int j=0;j<pos[0];j++) {
            //    cout<<pos[j+1]<<endl;
                file.seekg(i+(pos[j+1]-1)*bo);
                file.read(cd(temp),bo);
                allbook[j]=temp;
                //cout<<temp.IBSN<<' '<<temp.name<<' '<<temp.name<<' '<<temp.author<<' '<<temp.quantity<<' '<<temp.keyword<<' '<<fixed<<setprecision(2)<<temp.price;
            }
       //     cout<<temp.IBSN<<endl;
        //    cout<<"show author"<<endl;
        //    author.showall();
            sort(allbook,allbook+pos[0],ISBNsort);
            for(int i=0;i<pos[0];i++)  showbook(allbook[i]);
            delete[] pos;
            delete[] allbook;
        }
        else if(kind=="finance"){
            string strtime=todos->front();
            todos->pop();
            int time=stringtodouble(strtime);
            operating.showfi(time);
        }
        else if(kind=="financeall") {
            operating.showfi(operating.getfin());
        }
        file.close();
    }
    void buy(queue<string>* todos){
        fstream file;//还有finance等
        file.open(aofb,ios::in|ios::out|ios::binary);
        string strISBN=todos->front();
        todos->pop();

        int *poss;
        try {
            poss = ISBN.find(strISBN.c_str());
        }catch(NotFound){delete todos;file.close();throw Error();}
        string strnum=todos->front();
        todos->pop();
        int num=stringtodouble(strnum);
        file.seekg(i+(poss[1]-1)*bo);
        book tmp;
        file.read(cd(tmp),bo);
        if(tmp.quantity<num){
            delete todos;
            file.close();
            throw Error();
        }
        tmp.quantity-=num;
        operating.insert(num*tmp.price);
        file.seekp(i+(poss[1]-1)*bo);
        file.write(cd(tmp),bo);
        cout<<fixed<<setprecision(2)<<num*tmp.price<<endl;
        file.close();
    }
    void logout(){
    //    cout<<operating.getpri()<<endl;
        operating.logout();
    }
};
class Stuff:public Customer{
private:
    const int priviledge=3;
protected:
    int pos;
public:
    Stuff()=default;
    ~Stuff(){};
    void select(queue<string>* todos){
        string strISBN=todos->front();
        int *poss;
        try {
            poss=ISBN.find(strISBN.c_str());
        }catch(NotFound){
            fstream file;
            file.open(aofb,ios::in|ios::out|ios::binary);
            int pos=operating.newbook();
         //   cout<<pos<<endl;
            ISBN.insert(strISBN.c_str(),pos);
            book temp;
            temp.keyword[0]='\0';
            temp.name[0]='\0';
            temp.author[0]='\0';
            temp.quantity=0;
          //  cout<<temp.IBSN<<endl;
          //  temp.IBSN;
            strcpy((temp.IBSN),strISBN.c_str());
            temp.price=0;
            file.seekp(0);
            file.write(cd(pos),i);
            file.seekp(i+(pos-1)*bo);
            file.write(cd(temp),bo);
          //  cout<<temp.name<<temp.author<<temp.keyword<<"??"<<endl;
           // delete[]poss;
            operating.setpos(pos);
            file.close();
           // ISBN.showall();
           delete todos;
            return;
        }
        operating.setpos(poss[1]);
        //cout<<poss[1]<<strISBN<<endl;
        delete[]poss;
    }
    void useradd(queue<string>* todos){
        fstream file;
        file.open(aofu,ios::in|ios::out|ios::binary);
        string userid=todos->front();
        todos->pop();
        int *poss;
        try {
            poss = users.find(userid.c_str());
        }catch(NotFound){//找不到
            //在user文件顶部塞一个
            int pos=operating.newuser();
            user newu;
            string passwd=todos->front();
            todos->pop();
            string strpri=todos->front();
            todos->pop();
            if(strpri[0]>'9'||strpri[0]<'0') delete todos,file.close(), throw Error();
          //  cout<<"strpri:"<<strpri<<endl;
            int pri=stringtodouble(strpri);
          //  cout<<pri<<endl;
            if(pri>operating.getpri()) delete todos,file.close(),throw Error();
            string name=todos->front();
            todos->pop();
            newu.priviledge=pri;
           // newu.userid=new char[userid.length()];
           // newu.passwd=new char[passwd.length()];
           // newu.name=new char[name.length()];
            strcpy(newu.userid,userid.c_str());
            strcpy(newu.passwd,passwd.c_str());
            strcpy(newu.name,passwd.c_str());
            file.seekp(i+(pos-1)*u);
            file.write(cd(newu),u);
           // delete[] newu.userid;
           // delete[] newu.name;
           // delete[] newu.passwd;
            file.seekp(0);
            file.write(cd(pos),i);
            users.insert(userid.c_str(),pos);
            file.close();
            //delete[]poss;
            delete todos;
            return;
        }
        delete[]poss;
        file.close();
        delete todos;
        throw Error();//已存在
    }
    void modify(queue<string>* todos){
        fstream file;
        file.open(aofb,ios::in|ios::out|ios::binary);
        int pos=operating.getpos();
        if(pos==-1)delete todos, file.close(),throw Error();
        book tmp;
        file.seekg(i+(pos-1)*bo);
        file.read(cd(tmp),bo);
        while(!(todos->empty())){
            string type=todos->front();
            todos->pop();
            if(type=="a"){
                string strauthor=todos->front();
                todos->pop();
                if(strlen(tmp.author)!=0)  author.deldata(tmp.author,pos);
                author.insert(strauthor.c_str(),pos);
               // cout<<"a"<<endl;
               // author.showall();
                strcpy(tmp.author,strauthor.c_str());
            }
            else if(type=="I"){
                string strIBSN=todos->front();
                todos->pop();
                bool flag=0;
                if(true){
                    int* poss=nullptr;
                    try{ISBN.find(strIBSN.c_str());}catch(NotFound) {
                        ISBN.deldata(tmp.IBSN, pos);
                        ISBN.insert(strIBSN.c_str(), pos);
                        //    ISBN.showall();
                        strcpy(tmp.IBSN, strIBSN.c_str());
                        flag = 1;
                    }
                    if(poss!=nullptr)  delete[] poss;
                }
                if(!flag) {//找到了
                    delete todos;
                    throw Error();
                }
            }
            else if(type=="n"){
                string strname=todos->front();
                todos->pop();
                if(strlen(tmp.name)!=0)
                    name.deldata(tmp.name,pos);
                name.insert(strname.c_str(),pos);
            //    name.showall();
                strcpy(tmp.name,strname.c_str());
            }
            else if(type=="k"){
                string strnum=todos->front();
                todos->pop();
                int num=stringtodouble(strnum);
               // cout<<tmp.IBSN<<' '<<tmp.keyword<<endl;
                if(strlen(tmp.keyword)!=0){
                //    cout<<"oldkey:"<<tmp.keyword<<endl;
                //cout<<"pos:"<<pos<<endl;
           //     keyword.showall();
                    int l=strlen(tmp.keyword);
                    string oldkey[20];
                    int nofok=0;
                    string key="";
                    for(int i=0;i<l;i++){
                        if(tmp.keyword[i]=='|')  oldkey[nofok]=key,nofok++,key="",i++;
                        key+=tmp.keyword[i];
                    }
                    oldkey[nofok]=key;
                    nofok++;
                  //  for(int i=0;i<nofok;i++) cout<<oldkey[i]<<endl;
                    for(int i=0;i<nofok;i++)keyword.deldata(oldkey[i].c_str(),pos);
                }
                string newkey=todos->front();
                todos->pop();
               // cout<<"point 4"<<endl;
               // cout<<"newkey:"<<newkey<<endl;
                string newkeyword=newkey;//全部
                string newkeys[20];//用来排序
                newkeys[0]=newkey;
                for(int i=1;i<num;i++){
                    newkey=todos->front();
                    todos->pop();
                    newkeys[i]=newkey; //调出
                    newkeyword+='|',newkeyword+=newkey;
                }
               // cout<<newkeyword<<num<<endl;
                sort(newkeys,newkeys+num);
                for(int i=0;i<num-1;i++) if(newkeys[i]==newkeys[i+1])delete todos,file.close(), throw Error();
                for(int i=0;i<num;i++) keyword.insert(newkeys[i].c_str(),pos);
              //  cout<<"point 5"<<endl;
                //keyword.showall();
                strcpy(tmp.keyword,newkeyword.c_str());
            }
            else if(type=="p"){
                string strprice=todos->front();
                todos->pop();
                double price;
                price=stringtodouble(strprice);
                tmp.price=price;
            }
        }
        file.seekp(i+(pos-1)*bo);
        file.write(cd(tmp),bo);
        file.close();
      //  cout<<"point 6"<<endl;
            delete todos;
        return;
    }
    void import(queue<string>* todos){
        fstream file;
        file.open(aofb,ios::in|ios::out|ios::binary);
        string strstrnum=todos->front();
        todos->pop();
        if(strstrnum[0]>'9'||strstrnum[0]<'0'){delete todos;file.close();throw Error();}
        int num=stringtodouble(strstrnum);
        double price=stringtodouble(todos->front());
        todos->pop();
        int pos=operating.getpos();
        if(pos==-1)delete todos,file.close(),throw Error();
        book temp;
        file.seekg(i+(pos-1)*bo);
        file.read(cd(temp),bo);
        temp.quantity+=num;
        operating.insert(-price);
        file.seekp(i+(pos-1)*bo);
        file.write(cd(temp),bo);
        file.close();
        return;
       // ISBN.showall();
    }
};
class Boss:public Stuff{
private:
    const int priviledge=7;
public:
    Boss()=default;
    ~Boss(){};
    void Delete(queue<string>* todos){
        fstream file;
        file.open(aofu,ios::in|ios::out|ios::binary);
        string userid=todos->front();
        todos->pop();
        int* pos;
        try{
            pos=users.find(userid.c_str());
        }catch(NotFound){
           // delete []pos;
            file.close();
            delete todos;
            throw Error();
            return;
        }
        if(operating.is_online(userid)){
            file.close();
            delete todos;
            throw Error();
            return;
        }
        users.deldata(userid.c_str(),pos[1]);
        file.close();
 //       users.showall();
        delete []pos;
        return;
    };
};
#endif //UNTITLED5_USER_H
