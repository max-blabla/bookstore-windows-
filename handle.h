//
// Created by 16963 on 2021/2/20.
//

#ifndef UNTITLED5_HANDLE_H
#define UNTITLED5_HANDLE_H
#include "bookstore.h"
#include <queue>
#define cd(x)  (char*)&x
#define MAXNUL  50
#define MAXNBL 80
myblocklist ISBN;
myblocklist name;
myblocklist keyword;
myblocklist author;
myblocklist users;
const int uid_len=30;
const int pwd_len=30;
const int u_n_len=30;
const int ISBN_len=20;
const int au_len=60;
const int name_lem=60;
const int keyw_len=60;
const int quantity_m=100000;
inline void paspace(string& todo,int& pos){
    while(todo[pos]==' ') pos++;
}
struct user{
    //int numofuser;
    int priviledge;
    char name[MAXNUL];
    char userid[MAXNUL];
    char passwd[MAXNUL];
};
struct book{
    int quantity;
    char IBSN[MAXNBL];
    char name[MAXNBL];
    char author[MAXNBL];
    char keyword[MAXNBL];
    double price;
};
class handle {
private:
    const int i = sizeof(int);
    const int b = sizeof(block);
    const int n = sizeof(node);
    const int u = sizeof(user);
    const int bo = sizeof(book);
    int userpri=0;
    int anoupri;
    string allbofile;
    string addufile;
    fstream abos;
    fstream aus;
    int numofboall = 0;
    int numofuall = 0;
    int slmainpos;
    finance fi;
    stack<pair<user, int>> userstack;//int为选中的ISBN的绝对位置
    unordered_map<string, int> count;//string为userid int为计数
    void abosinsert(book boo) {
        abos.seekp(0);
        numofboall++;
        abos.write(cd(numofboall), i);
        abos.seekp(i + numofboall * bo);
        abos.write(cd(boo), u);
    }

    void ausinsert(user& us) {
        aus.seekp(0);
        numofuall++;
        aus.write(cd(numofuall), i);
        aus.seekp(i);
        aus.write(cd(us), u);
    }

public:
    handle(){}
    void initial(const char *adname,const char *auname,const char *ISBNname,
                 const char *namename,const char *keyname, const char *authorname,
                 const char *usersname,const char* financename,const char* ISBNpool,
                 const char* namepool,const char* keypool,const char* authorpool,
                 const char* userpool) {
        abos.open(adname, ios::in | ios::out | ios::binary);
        aus.open(auname, ios::in | ios::out | ios::binary);
        if (!abos.is_open()) {
            ofstream f1;
            f1.open(adname, ios::out | ios::binary);
            ofstream f2;
            f2.open(auname, ios::out | ios::binary);
            f1.seekp(0);
            int zero = 0;
            f1.write(cd(zero), i);
            numofboall=0;
            f2.seekp(0);
            int one=1;
            f2.write(cd(one), i);
            numofuall=1;
            ISBN.initial(ISBNname, ISBNpool);
            name.initial(namename, namepool);
            author.initial(authorname, authorpool);
            keyword.initial(keyname, keypool);
            users.initial(usersname, userpool);
            fi.initialf(financename);
            string strname = "root";
            string passwd = "sjtu";
            string userid = "root";
            user root;
            strcpy(root.userid, userid.c_str());
            //cout<<root.userid<<endl;
            //cout<<strname<<endl;
            strcpy(root.name, strname.c_str());
            strcpy(root.passwd, passwd.c_str());
            root.priviledge = 7;
            users.insert(strname.c_str(), 1);
            f2.write(cd(root),u);
            f1.close();
            f2.close();
        } else {
            ISBN.initial(ISBNname, ISBNpool);
            name.initial(namename, namepool);
            author.initial(authorname, authorpool);
            keyword.initial(keyname, keypool);
            users.initial(usersname, userpool);
            fi.initialf(financename);
            abos.seekg(0);
            abos.read(cd(numofboall), i);
            aus.seekg(0);
            aus.read(cd(numofuall), i);
           // cout<<numofuall<<endl;
        }
        user zero;
        zero.priviledge=0;
        zero.userid[0]='\0';
        zero.passwd[0]='\0';
        zero.name[0]='\0';
        userstack.push(pair<user,int>(zero,-1));
        abos.close();
        aus.close();
    }
    int judge(string order) {
        if(order=="su") return 1;
        else if(order=="register")  return 2;
        else if(order=="logout")  return 3;
        else if(order=="buy") return 4;
        else if(order=="show")return 5;
        else if(order=="passwd")  return 6;
        else if(order=="select")   return 7;
        else if(order=="modify")  return 8;
        else if(order=="import")  return 9;
        else if(order=="useradd")  return 10;
        else if(order=="delete")  return 11;
        else if(order=="quit"||order=="exit") return 0;
        else{
            return -1;
        }
    }
    int getpri(){
      //  cout<<anoupri<<endl;
        return userpri;
    }
    queue<string>* is_correct(int op,const char* todos,int length){//根据op 读入，拆解字符串
        queue<string>* streamstr=new queue<string>;
        switch (op) {
            case 1:{
                string todo=todos;
                string userid;
                int mpos=0;//待拆解字符串
                while(todo[mpos]!=' '&&todo[mpos]!='\0')
                    userid+=todo[mpos++];
                paspace(todo,mpos);
                string passwd="";
                while(todo[mpos]!=' '&&todo[mpos]!='\0')
                    passwd+=todo[mpos++];
                //pos=todo.find(' ');
                if(passwd=="v8^"){
                    if(userid.length()>uid_len)  delete streamstr,throw Error();
                    if(userpri==7) streamstr->push(userid);
                    else {delete streamstr;throw Error();}
                }
                else{
                   //加长度疑问
                    if(userid.length()>uid_len)  delete streamstr,throw Error();
                    if(passwd.length()>pwd_len)  delete streamstr,throw Error();
                    streamstr->push(userid);
                    streamstr->push(passwd);
                    //strncpy(userid.c_str(),todo.c_str(),pos);
                }
                return streamstr;
            }
            case 2:{
                string todo=todos;
                string userid="";
                string passwd="";
                string name="";
                int mpos=0;
                while(todo[mpos]!=' '&&todo[mpos]!='\0') userid+=todo[mpos++];
                paspace(todo,mpos);
                while(todo[mpos]!=' '&&todo[mpos]!='\0') passwd+=todo[mpos++];
                paspace(todo,mpos);
                while(todo[mpos]!=' '&&todo[mpos]!='\0') name+=todo[mpos++];
                if(userid.length()>uid_len) delete streamstr,throw Error();
                if(passwd.length()>pwd_len) delete streamstr,throw Error();
                if(name.length()>u_n_len)  delete streamstr,throw Error();
                streamstr->push(userid);
                streamstr->push(passwd);
                streamstr->push(name);
                return streamstr;
            }
            case 3:{
             //   cout<<userpri<<endl;
                if(userpri<1){delete streamstr,throw Error();}
                delete streamstr;
                return nullptr;
            }
            case 4:{
                if(userpri<1) {delete streamstr,throw Error();}
                string todo=todos;
                string ISBN="";
                string quantity="";//待拆解字符串
                int mpos=0;
                while(todo[mpos]!=' '&&todo[mpos]!='\0') ISBN+=todo[mpos++];
                paspace(todo,mpos);
                while(todo[mpos]!=' '&&todo[mpos]!='\0') quantity+=todo[mpos++];
                if(ISBN.length()>ISBN_len)   delete streamstr,throw Error();
                if(quantity[0]>'9'||quantity[0]<'0') delete streamstr, throw Error();
                streamstr->push(ISBN);
                streamstr->push(quantity);
                //strncpy(userid.c_str(),todo.c_str(),pos);
                return streamstr;
            }
            case 5:{
                if(userpri<1)  {delete streamstr,throw Error();}
                string todo=todos;
                int mpos=0;
                string type;
                while(todo[mpos]!='='&&todo[mpos]!=' '&&todo[mpos]!='\0') type+=todo[mpos++];
                if(type=="v8^")  {streamstr->push("showall");return streamstr;}
                if(todo[0]=='-'){
                    if(todo[1]=='I'){
                        mpos++;
                        string ISBN="";
                        while(todo[mpos]!=' '&&todo[mpos]!='\0') ISBN+=todo[mpos++];
                        if(ISBN.length()>ISBN_len) delete streamstr,throw Error();
                        streamstr->push("I");
                        streamstr->push(ISBN);
                    }
                    else if(todo[1]=='a'){
                        mpos+=2;
                        string author="";
                        while(todo[mpos]!='"'&&todo[mpos]!='\0') author+=todo[mpos++];
                        if(author.length()>au_len) delete streamstr,throw Error();
                        streamstr->push("a");
                        streamstr->push(author);
                    }
                    else if(todo[1]=='k'){
                        mpos+=2;
                        string keyword="";
                        while(todo[mpos]!='"'&&todo[mpos]!='\0') keyword+=todo[mpos++];
                        //strcpy(keyword,todo.c_str()+pos+2);
                        if(keyword.length()>keyw_len) delete streamstr,throw Error();
                        streamstr->push("k");
                        streamstr->push(keyword);
                    }
                    else if(todo[1]=='n'){
                        mpos+=2;
                        string name="";
                        while(todo[mpos]!='"'&&todo[mpos]!='\0') name+=todo[mpos++];
                        if(name.length()>name_lem) delete streamstr,throw Error();
                        streamstr->push("n");
                        streamstr->push(name);
                    }
                }
                else if(todo[0]=='f'){
                    if(userpri<7) {delete streamstr,throw Error();}
                    mpos++;
                    string time;
                    while(todo[mpos]!=' '&&todo[mpos]!='\0') time+=todo[mpos++];
                    if(time=="v8^"){
                        streamstr->push("financeall");
                    }
                    else{
                        streamstr->push("finance");
                        streamstr->push(time);
                    }
                }
                else  delete streamstr,throw Error();
                return streamstr;
            }
            case 6:{//repasswd
                if(userpri<1)  {delete streamstr,throw Error();}
                string todo=todos;
                string userid="";
                int mpos=0;//待拆解字符串
                while(todo[mpos]!=' '&&todo[mpos]!='\0') userid+=todo[mpos++];
                if(userid.length()>uid_len) { delete streamstr,throw Error();}
                streamstr->push(userid);
                string newpasswd;
                if(userpri==7){
                    paspace(todo,mpos);
                    while(todo[mpos]!=' '&&todo[mpos]!='\0') newpasswd+=todo[mpos++];
                    if(newpasswd.length()>pwd_len)  delete streamstr,throw Error();
                    streamstr->push(newpasswd);
                    return streamstr;
                }
                else{
                    string oldpasswd;
                    paspace(todo,mpos);
                    while(todo[mpos]!=' '&&todo[mpos]!='\0') oldpasswd+=todo[mpos++];
                    paspace(todo,mpos);
                    while(todo[mpos]!=' '&&todo[mpos]!='\0') newpasswd+=todo[mpos++];
                    if(oldpasswd.length()>pwd_len) { delete streamstr,throw Error();}
                    if(newpasswd.length()>uid_len)  delete streamstr,throw Error();
                    streamstr->push(oldpasswd);
                    streamstr->push(newpasswd);
                    //strncpy(userid.c_str(),todo.c_str(),pos);
                    return streamstr;
                }
            }
            case 7:{//select
                if(userpri<3){delete streamstr,throw Error();}
                string todo=todos;
                string ISBN="";
                int mpos=0;
                while(todo[mpos]!=' '&&todo[mpos]!='\0') ISBN+=todo[mpos++];
                if(ISBN.length()>ISBN_len) delete streamstr,throw Error();
                streamstr->push(ISBN);
                return streamstr;
            }
            case 8:{//modify
                if(userpri<3)  {delete streamstr,throw Error();}
                string todo=todos;
                string name;
                string ISBN;
                string author;
                string price;
                string keyword[20];
                int mpos=0;
                int numofkey=0;
                while(todo[mpos]!='\0'){
                    if(todo[mpos]=='-'){
                        if(todo[mpos+1]=='I'){
                            mpos+=6;
                            while(todo[mpos]!=' '&&todo[mpos]!='\0')  ISBN+=todo[mpos++];
                            if(ISBN.length()>ISBN_len) delete streamstr,throw Error();
                            streamstr->push("I");
                            streamstr->push(ISBN);
                        }
                        else if(todo[mpos+1]=='a'){
                            mpos+=9;
                            while(todo[mpos]!='"'&&todo[mpos]!='\0'&&todo[mpos]!=' ')  author+=todo[mpos++];
                            if(author.length()>au_len) delete streamstr,throw Error();
                            streamstr->push("a");
                            streamstr->push(author);
                        }
                        else if(todo[mpos+1]=='k'){
                            mpos+=10;
                            int length=0;
                            string akey="";
                            while(todo[mpos]!='"'&&todo[mpos]!=' '&&todo[mpos]!='\0') {
                                akey+=todo[mpos];
                                mpos++,length++;
                                if(todo[mpos]=='|')keyword[numofkey]=akey,akey="",mpos++,length++,numofkey++;
                            }
                            if(length>keyw_len) delete streamstr,throw Error();
                            keyword[numofkey]=akey;
                            numofkey++;
                            streamstr->push("k");
                            streamstr->push(to_string(numofkey));
                            for(int j=0;j<numofkey;j++)  streamstr->push(keyword[j]);
                        }
                        else if(todo[mpos+1]=='n'){
                            mpos+=7;
                            while(todo[mpos]!='"'&&todo[mpos]!='\0'&&todo[mpos]!=' ') name+=todo[mpos++];
                            if(name.length()>name_lem) delete streamstr,throw Error();
                            streamstr->push("n");
                            streamstr->push(name);
                        }
                        else if(todo[mpos+1]=='p'){
                            mpos+=7;
                            while(todo[mpos]!=' '&&todo[mpos]!='\0')  price+=todo[mpos++];
                            streamstr->push("p");
                            streamstr->push(price);
                        }
                    }
                    mpos++;
                }
                return streamstr;
            }
            case 9:{//import
                if(userpri<3)  {delete streamstr,throw Error();}
                string strnum="";
                string strprice="";
                string todo=todos;
                int mpos=0;
                while(todo[mpos]!=' '&&todo[mpos]!='\0')  strnum+=todo[mpos++];
                //double price;
               //double price;
               paspace(todo,mpos);
               while(todo[mpos]!=' '&&todo[mpos]!='\0')  strprice+=todo[mpos++];
                streamstr->push(strnum);
                streamstr->push((strprice));
                //streamstr->push(to_string(price));
                return streamstr;
            }
            case 10:{//useradd
                if(userpri<3) {delete streamstr,throw Error();}
                string todo=todos;
                string userid;
                string passwd;
                string name;
                string strpri;
                int mpos=0;
                while(todo[mpos]!=' '&&todo[mpos]!='\0') userid+=todo[mpos++];
                if(userid=="v8^") delete streamstr,throw Error();
                if(userid.length()>uid_len)  delete streamstr,throw Error();
                paspace(todo,mpos);
                while(todo[mpos]!=' '&&todo[mpos]!='\0') passwd+=todo[mpos++];
                if(passwd=="v8^") delete streamstr,throw Error();
                if(passwd.length()>pwd_len)  delete streamstr,throw Error();
                paspace(todo,mpos);
                while(todo[mpos]!=' '&&todo[mpos]!='\0') strpri+=todo[mpos++];
                if(strpri=="v8^") delete streamstr,throw Error();
                if(strpri.length()>1)  delete streamstr,throw Error();
                paspace(todo,mpos);
                while(todo[mpos]!=' '&&todo[mpos]!='\0') name+=todo[mpos++];
                if(name=="v8^") delete streamstr,throw Error();
                if(name.length()>u_n_len)  delete streamstr,throw Error();
                streamstr->push(userid);
                streamstr->push(passwd);
                streamstr->push(strpri);
                streamstr->push(name);
                return streamstr;
            }
            case 11:{//delete
                if(userpri<7) {delete streamstr,throw Error();}
                string todo=todos;
                string userid="";
                int mpos=0;
                while(todo[mpos]!=' '&&todo[mpos]!='\0') userid+=todo[mpos++];
                if(userid.length()>uid_len) delete streamstr,throw Error();
                streamstr->push(userid);
                return streamstr;
            }
            case 0:{delete streamstr;return nullptr;}
            case -1:{
                //cout<<"Invalid"<<endl;
                string err="";
                string todo=todos;
                int mpos=0  ;
                while(todo[mpos]!='\0'){
                    while(todo[mpos]!=' '&&todo[mpos]!='\0')
                        err+=todo[mpos++];
                    paspace(todo,mpos);
                    cout<<"Invalid"<<endl;
                    err="";
                    if(err=="v8^") break;
                }
                delete streamstr;
                return nullptr;
            }
        }
    }
    void login(user& u){
        slmainpos=-1;
        userstack.push(pair<user,int>(u,slmainpos));
        string userid=u.userid;
     //   cout<<"login:"<<u.userid<<' '<<slmainpos<<endl;
     //   cout<<userid<<endl;
        if(!count.count(userid)){
            count.insert(pair<string,int>(userid,1));
         //   cout<<"?"<<endl;
        }
        else{
            pair<string,int>p=*(count.find(userid));
            count.erase(userid);
            p.second++;
            count.insert(p);
        }
        userpri=u.priviledge;
     //  cout<<"userpri:"<<userpri<<endl;
      //  cout<<u.priviledge<<endl;
    };
    void logout(){
        pair<user,int>p=userstack.top();
        userstack.pop();
        //cout<<p.first.userid<<endl;
       // cout<<p.second<<endl;
        //cout<<count.count(p.first.userid)
        pair<string,int>p2=*(count.find(p.first.userid));
        count.erase(p2.first);
        p2.second--;
        if(p2.second>0) count.insert(p2);
        p=userstack.top();
     //   cout<<p.first.userid<<' '<<p.second<<endl;
        slmainpos=p.second;
        userpri=p.first.priviledge;
    };
    bool is_online(string userid){
        if(count.count(userid)) return 1;
        else 0;
    }
    void showfi(int time){
        fi.show(time);
    }
    void insert(double money) {
        fi.insertf(money);
    }
    int getfin(){
        return fi.getsize();
    }
    void setpos(int pos){
        pair<user,int>p=userstack.top();
        slmainpos=pos;
        p.second=pos;
        userstack.pop();
        userstack.push(p);
        //cout<<"setpos"<<slmainpos<<endl;
    }
    int getpos(){
        return slmainpos;
    }
    int getab(){
        return numofboall;
    }
    int newbook(){
        return  (++numofboall);
    }
    int newuser(){
    //    cout<<numofuall<<endl;

        return  (++numofuall);
    }
};
#endif //UNTITLED5_HANDLE_H
