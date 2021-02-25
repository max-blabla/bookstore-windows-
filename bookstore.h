//
// Created by 16963 on 2021/2/19.
//

#ifndef UNTITLED4_BOOKSTORE_H
#define UNTITLED4_BOOKSTORE_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <stack>
#include<unordered_map>
#include<vector>
#include<iomanip>

using namespace std;
#define cd(x)  (char*)&x
struct node{
    char key[100];
    int pos;
};
class Error{};
class NotFound{};
class block{
    friend class myblocklist;
private:int next=0,num=0;
    node data[400];
public:
    block()=default;
};
class myblocklist{
private:
    const int i=sizeof(int);
    const int b=sizeof(block);
    const int n=sizeof(node);
    // int pool[1000],tot=0;
    int numofblock=0;
    int head=0;
    string datafile;
    string poolfile;
    fstream ds;
    fstream ps;
    // block* allblo;
    void maintain(){
        //其它时候只删不增的屑
        openstream();
        count=0;
        block curblo;
        block nxtblo;
        int cur;
        cur=head;
        ds.seekg((cur-1)*b+2*i);
        ds.read(cd(curblo),b);
        if(!curblo.next){ closestream();return;}
        ds.seekg((curblo.next-1)*b+2*i);
        ds.read(cd(nxtblo),b);
        closestream();
        openstream();

        while(1){
            if(curblo.num+nxtblo.num<150) {
                merge(cur, curblo.next);
                if (!nxtblo.next) break;
                cur = nxtblo.next;
                ds.seekg((cur - 1 * b) + 2 * i);
                ds.read(cd(curblo), b);
                if(!curblo.next) break;
                ds.seekg((curblo.next - 1) * b + 2 * i);
                ds.read(cd(nxtblo),b);
            }
            else{
                cur=curblo.next;
                ds.seekg((cur-1)*b+2*i);
                ds.read(cd(curblo),b);
                if(!curblo.next) break;
                ds.seekg((curblo.next-1)*b+2*i);
                ds.read(cd(nxtblo),b);
            }
        }
        count++;
        closestream();
    }
    void merge(const int blo1,int blo2){//将2粘到1上
        block tmp1;
        block tmp2;
        //free(blo2);
        ds.seekg((blo1-1)*b+2*i);
        ds.read(cd(tmp1),b);
        ds.seekg((blo2-1)*b+2*i);
        ds.read(cd(tmp2),b);
        for(int i=tmp1.num;i<tmp1.num+tmp2.num;i++) tmp1.data[i]=tmp2.data[i-tmp1.num];
        tmp1.num+=tmp2.num;
        tmp1.next=tmp2.next;
        //    cout<<tmp1.next<<tmp2.next<<endl;
        free(blo2);
        numofblock--;
        //   cout<<"have merged"<<endl;
        //   cout<<tmp1.num<<tmp2.num<<endl;
        //   for(int i=0;i<tmp1.num;i++) cout<<tmp1.data[i].key<<endl;
        ds.seekp((blo1-1)*b+2*i);
        ds.write(cd(tmp1),b);
        ds.seekp(0);
        ds.write(cd(numofblock),i);
    }
    /*int compare(const char* a,const char* b){//左小为-1
        int a_len=strlen(a);
        int b_len=strlen(b);
        for(int i=0;i<min(a_len,b_len);i++){
            if(a[i]>b[i])
                return 1;
            else if(a[i]<b[i])
                return -1;
        }
        if(a_len>b_len)
            return 1;
        else if(a_len<b_len)
            return -1;
        return 0;
    }*/
    int getfree_num(){
        int free_num;
        ps.seekg(0);
        ps.read(cd(free_num),i);
        return free_num;
    }
    void writefree_num(int free_num){
        ps.seekp(0);
        ps.write(cd(free_num),i);
    }
    int left_bound(node* node,const char* str,int capacity){
        int left=0;
        int right=capacity;
        while(left<right){
            int mid=(left+right)/2;
            // cout<<node[mid].key<<endl;
            if((strcmp(str,node[mid].key)==0)){
                right=mid;
            }
            else if(strcmp(str,node[mid].key)<0)
                right=mid;
            else if(strcmp(str,node[mid].key)>0)
                left=mid+1;
        }
        if(left==capacity) return -1;
        return left;
    }
    int  right_bound(node* node, const char* str,int capacity){
        int left=0;
        int right=capacity;
        while(left<right){
            int mid=(left+right)/2;
            //   cout<<str<<endl;
            // cout<<node[mid].key<<endl;
            if((strcmp(str,node[mid].key)==0))
                left=mid+1;
            else if(strcmp(str,node[mid].key)<0)
                right=mid;
            else if(strcmp(str,node[mid].key)>0)
                left=mid+1;
        }
        if(right==0) return 0;
        // if(right==capacity) return capacity-1;
        return strcmp(node[left-1].key,str)==0?(left-1):left;
    }
    void openstream(){
        ds.open(datafile,fstream::binary|fstream ::out|fstream::in);
        ps.open(poolfile,fstream::binary|fstream ::out|fstream::in);
    }
    void closestream(){
        ds.close();
        ps.close();
    }
    void showpool(){
        int num;
        ps.seekg(0);
        ps.read(cd(num),i);
        cout<<"free:"<<num<<endl;
        int pool;
        for(int j=0;j<num;j++){
            ps.read(cd(pool),i);
            cout<<pool<<endl;
        }

    }
    int mem_new(){
        int free= getfree_num();
        if(!free){
            ps.seekp(0);
            ps.write((char*)&(numofblock),i);
            return numofblock;
        }
        else{
            int pos;
            ps.seekg(free*i);
            ps.read((char*)&(pos),i);
            free--;
            writefree_num(free);
            //  cout<<"mem_new"<<endl;
            //  showpool();
            return pos;
        }
        showpool();
    }
    void free(int pos){
        // openstream();
        int free= getfree_num();
        ps.seekp((free+1)*i);
        ps.write((char*)&pos,i);
        free++;
        writefree_num(free);
        //  closestream();
        //  cout<<"free"<<endl;
       // showpool();
    }
    int count=0;
public:
    myblocklist()=default;
    void initial(string storename,string poolname){
        //得到numofblock
        //得到开头
        datafile=storename;
        poolfile=poolname;
        openstream();
        //ps.open(poolname,ios::out|ios::trunc|ios::binary);
        if(!ps.is_open()) {
            ofstream f1;
            f1.open(poolname,ios::out|ios::binary);
            f1.seekp(0);
            int free=1000;
            f1.write((char*)&(free),i);
            int temp;
            for(int j=0;j<1000;j++){
                temp=1000-j;f1.write(cd(temp),i);
            }
            ofstream f2;
            f2.open(storename,ios::out|ios::binary);
            f2.seekp(0);
            int zero=0;
            f2.write((char*)&zero,i);
            f2.write((char*)&zero,i);
            head=0;
            numofblock=0;
            f1.close();
            f2.close();
            return;
        }
        else
        {
            ds.seekg(0);
            ds.read(cd(numofblock),i);
            ds.read(cd(head),i);
        }
        closestream();
    }
    void insert(const char* key,int mainpos){//必定在原来顺序栈中没有//done
        if(count>=300) maintain();
        openstream();
        //cout<<numofblock<<endl;
        if(numofblock==0){
            head=mem_new();
            numofblock++;
            //head=newpos;
            ds.write(cd(numofblock),i);
            ds.write(cd(head),i);//写入头
            block newblo;
            newblo.next=0;
            newblo.num=1;
            strcpy(newblo.data[0].key,key);
            newblo.data[0].pos=mainpos;
            ds.write(cd(newblo),b);
            count++;
            closestream();
            return;
        }
        int pos;
        int cur=head;
        int nxt;
        node tmpnode;
        ds.seekg(2*i+(head-1)*b+2*i);
        ds.read(cd(tmpnode),n);
        // cout<<tmpnode.key<<endl;
        if(strcmp(tmpnode.key,key)>0) cur=head;
        else{
            while(1){
                //pre=blockpos;
                ds.seekg(2*i+(cur-1)*b);
                ds.read(cd(nxt),i);
                if(!nxt) break;
                ds.seekg(2*i+(nxt-1)*b+2*i);
                ds.read(cd(tmpnode),n);
                if(strcmp(tmpnode.key,key)>=0) break;
                cur=nxt;
            }
        }
        //找位置
        block curblo;
        block newblo;
        ds.seekg(2*i+(cur-1)*b);
        // cout<<ds.tellg()<<endl;
        ds.read(cd(curblo),b);
        int nodepos;
        // cout<<curblo.data[curblo.num-1].key<<' '<<key<<endl;
        if(strcmp(curblo.data[curblo.num-1].key,key)<=0) nodepos=curblo.num;
        else if(strcmp(curblo.data[0].key,key)>0) nodepos=0;
        else{
            for(int i=0;i<curblo.num-1;i++){
                if(strcmp(curblo.data[i].key,key)==0) {nodepos=i;break;}
                else if(strcmp(curblo.data[i].key,key)<0&&strcmp(curblo.data[i+1].key,key)>0) {nodepos=i+1;break;}
            }
        }
        if(curblo.num<200){
            for(int j=curblo.num;j>nodepos;j--) curblo.data[j]=curblo.data[j-1];
        //    cout<<nodepos<<endl;
            strcpy(curblo.data[nodepos].key,key);
            curblo.data[nodepos].pos=mainpos;
            curblo.num++;
            //  curblo.next=mem_new();
            ds.seekp(2*i+b*(cur-1));
            ds.write(cd(curblo),b);
            /* for(int i=0;i<curblo.num;i++){
                 cout<<curblo.data[i].key<<curblo.data[i].pos<<endl;
             }*/
            count++;
            closestream();
            return;
        }
        else{
            if(nodepos==curblo.num) {
                int half=curblo.num/2;
                for(int j=half;j<curblo.num;j++){
                    newblo.data[j-half]= curblo.data[j];
                    //<<j<<endl;
                }
                newblo.num=1+curblo.num-half;
                // cout<<newblo.num<<endl;
                // cout<<curblo.num<<endl;
                curblo.num=half;
                strcpy(newblo.data[newblo.num-1].key,key);
                newblo.data[newblo.num-1].pos=mainpos;
                //for (int i = 0; i < curblo.num; i++) {
                // cout << curblo.data[i].key << curblo.data[i].pos << endl;
                //}
                //cout<<endl;
                // for (int i = 0; i < newblo.num; i++) {cout<<newblo.data[i].key<<newblo.data[i].pos<<endl;}
            }
            else {
                //cout<<nodepos<<endl;
                for (int j = nodepos; j < curblo.num; j++) newblo.data[j - nodepos] = curblo.data[j];
                strcpy(curblo.data[nodepos].key, key);
                curblo.data[nodepos].pos = mainpos;
                newblo.num = curblo.num - nodepos;//
                // cout<<newblo.num<<endl;
                curblo.num = nodepos + 1;
                //  for (int i = 0; i < curblo.num; i++) cout << curblo.data[i].key << curblo.data[i].pos << endl;

                //cout<<endl;
                //for (int i = 0; i < newblo.num; i++) {cout<<newblo.data[i].key<<newblo.data[i].pos<<endl;}
            }
            newblo.next=curblo.next;
            curblo.next=mem_new();
            //  cout<<curblo.next<<endl;
            //  cout<<newblo.next<<endl;
            ds.seekp(2*i+b*(cur-1));
            ds.write(cd(curblo),b);
            ds.seekp(2*i+(curblo.next-1)*b);
            ds.write(cd(newblo),b);
            numofblock++;
            ds.seekp(0);
            ds.write(cd(numofblock),i);
            // cout<<numofblock<<endl;
        }
        count++;
        closestream();
    }
    void deldata(const char* key,int mainpos){//pos 为顺序中位置
        if(count>=300) maintain();
        openstream();
        int blockpos=head;
        int pre=-1;
        int nxt;
        //int num;
        node tmpnode;
        // cout<<numofblock<<endl;
        if(numofblock==0) {
            cout<<"here"<<endl;
            closestream();
            throw NotFound();
        }
        ds.seekg(2*i+(head-1)*b+2*i);
        ds.read(cd(tmpnode),n);
        //  cout<<tmpnode.key<<endl;
        //cout<<"point 1"<<endl;
        //       closestream();
        //       this->showall();
        //      openstream();
        if(strcmp(tmpnode.key,key)>0) {  closestream();
            //    cout<<"there"<<endl;
            throw NotFound();}
        closestream();
        // this->showall();
        openstream();
        while(1){
            //pre=blockpos;
            ds.seekg(2*i+(blockpos-1)*b);
            ds.read(cd(nxt),i);
            if(!nxt) break;
            ds.seekg(2*i+(nxt-1)*b+2*i);
            ds.read(cd(tmpnode),n);
            //  cout<<tmpnode.key<<endl;
            if(strcmp(tmpnode.key,key)>=0) break;
            pre=blockpos;
            blockpos=nxt;
        }//得到key所在位置以及key前的位置
        // ds.seekg(2*sizeof(int));//移动到key对应block
        count++;
        ds.seekg((blockpos-1)*b+2*i);
        block blo;
        ds.read(cd(blo),b);//得到block
        //cout<<blockpos<<endl;
        int nodepos;
        bool same=0;
        while(true){//找到mainpos在数组中位置
            // cout<<blo.next<<' '<<blo.num<<endl;
            //cout<<mainpos<<endl;
            for(int i=0;i<blo.num;i++){
                if(strcmp(blo.data[i].key,key)==0&&blo.data[i].pos==mainpos){ same=1,nodepos=i;break;}
            }
            if(same) break;
            if(blo.next){
                ds.seekg(2*i+(blo.next-1)*b+2*i);
                ds.read(cd(tmpnode),n);
                // cout<<"tmpnode:"<<tmpnode.key<<endl;
                if(strcmp(tmpnode.key,key)!=0) break;
                else{
                    ds.seekg(2*i+(blo.next-1)*b);
                    pre=blockpos;
                    blockpos=blo.next;
                    ds.read(cd(blo),b);}
            }
            else break;
            //     cout<<tmpnode.key<<' '<<key<<endl;

        }
        if(!same){closestream(),throw NotFound();}
        // cout<<"nodepos:"<<nodepos<<endl;
        char unuse[100]="";
        count++;
        if(nodepos==blo.num-1) strcpy(blo.data[nodepos].key,unuse),blo.data[nodepos].pos=0;
        else for(int i=nodepos;i<blo.num;i++)  blo.data[i]=blo.data[i+1];//简单消除
        blo.num--;//data数目减少
        if(blo.num==0){
            if(blockpos==head){//头结点对应blockpos弄没了
                head=blo.next;
                ds.seekp(i);
                ds.write(cd(head),sizeof(int));
            }
            else{
                ds.seekp(2*i+b*(pre-1));
                ds.write(cd(blo.next),sizeof(int));
            }
            ds.seekp(b*(blockpos-1)+2*i);
            ds.write(cd(blo),b);
            free(blockpos);//释放内存
            numofblock--;
            ds.seekp(0);
            closestream();
            count++;
            return;
        }
        //未删完的
        ds.seekp(b*(blockpos-1)+2*i);
        ds.write(cd(blo),b);
        closestream();
    }//done
    int* find(const char* key){//返回顺序位置
        if(count>=300) maintain();
        openstream();
        block curblo;
        // int headpos=getblockposition(key);
        int headpos=head;
        int nxt;
        node tmpnode;
        ds.seekg(2*i+(head-1)*b+2*i);
        ds.read(cd(tmpnode),n);
        closestream();
        // cout<<tmpnode.key<<endl;
        //  cout<<"key:"<<key<<endl;
        //  this->showall();
        openstream();
        if(strcmp(tmpnode.key,key)>0) {  closestream();
            throw NotFound();}
        while(1){
            //pre=blockpos;
            ds.seekg(2*i+(headpos-1)*b);
            ds.read(cd(nxt),i);
            if(!nxt) break;
            ds.seekg(2*i+(nxt-1)*b+2*i);
            ds.read(cd(tmpnode),n);
            if(strcmp(tmpnode.key,key)>=0) break;
            headpos=nxt;
        }
        ds.seekg(2*sizeof(int)+(headpos-1)*b);
        ds.read(cd(curblo),sizeof(block));//找到头块
        //int blockpos=getblockposition(key);
        pair<int,int>posarray[5];
        int arraynum=0;
        pair<int,int>p;
        bool last=0;
        while(true) {
            //   cout<<"?"<<endl;
            bool left = 0;
            bool right = 0;
            for (int i = 0; i < curblo.num - 1; i++) {
                //  cout<<curblo.data[i].key<<' '<<curblo.data[i+1].key<<' '<<i<<endl;
                if (strcmp(curblo.data[i].key, key) < 0 && strcmp(curblo.data[i + 1].key, key) == 0 && !left)
                    posarray[arraynum].first = i + 1, left = 1;
                if (strcmp(curblo.data[i].key, key) == 0 && strcmp(curblo.data[i + 1].key, key) > 0 && !right)
                    posarray[arraynum].second = i, right = 1;
            }
            if (strcmp(curblo.data[0].key, key) == 0) posarray[arraynum].first = 0, left = 1;
            if (strcmp(curblo.data[curblo.num - 1].key, key) == 0) posarray[arraynum].second = curblo.num - 1;
            if (left) arraynum++;
            else headpos=curblo.next;
            if (curblo.next) {
                ds.seekg(2 * i + (curblo.next - 1) * b);
                ds.read(cd(curblo), b);
                if (strcmp(curblo.data[0].key, key) != 0) break;
            } else break;
            // if(!curblo.next) break;
        }
        if(arraynum==0){//找不到
            closestream();
            throw NotFound();
        }
        int capacity=0;
        for(int i=0;i<arraynum;i++) capacity+=posarray[i].second-posarray[i].first+1;
        //   cout<<capacity<<endl;
        int *poss;
        int poss_pos=1;
        poss=new int[capacity+1];
        poss[0]=capacity;
        ds.seekg(2*i+(headpos-1)*b);
        ds.read(cd(curblo),b);
        for(int j=0;j<arraynum;j++){//将绝对位置赋给位置数组
            for(int k=posarray[j].first;k<=posarray[j].second;k++){
                poss[poss_pos]=curblo.data[k].pos;
                poss_pos++;
            }
            ds.seekg(2*i+(curblo.next-1)*b);
            ds.read(cd(curblo),b);
        }

        closestream();
        return poss;
    }
    int* findall(int allnum){

        openstream();
        if(count>=300) maintain();

        if(head==0) closestream(),throw NotFound();
        int* allpos=new int[allnum];
        block curblo;
        //ds.seekg(2*i);
        ds.seekg(2*i+(head-1)*b);
        //  cout<<ds.tellg()<<endl;
        ds.read(cd(curblo),b);
        int cur=head;
        int pos=0;
        while(1){
            // cout<<curblo.num<<curblo.next<<endl;
            for(int i=0;i<curblo.num;i++)
                allpos[pos]=curblo.data[i].pos,pos++;
            if(curblo.next==0)
                break;
            else{
                cur=curblo.next;
                ds.seekg(2*i+(cur-1)*b);
                ds.read(cd(curblo),b);
            }
        }
        closestream();
        return allpos;

    }
    void zero(){
        ds.open(datafile,ios::trunc|ios::in|ios::out|ios::binary);
        ps.open(datafile,ios::trunc|ios::in|ios::out|ios::binary);
        ds.close();
        ps.close();
    }
    void showall(){
        openstream();
        cout<<numofblock<<endl;
        int h;
        int n;
        block tb;
        ds.seekg(0);
        ds.read(cd(n),i);
        ds.read(cd(h),i);
        cout<<"all:"<<n<<' '<<h<<endl;
        ds.read(cd(tb),b);
        while(true){
            cout<<"tb:"<<tb.next<<' '<<tb.num<<endl;
            for(int i=0;i<tb.num;i++){
                cout<<tb.data[i].pos<<' '<<tb.data[i].key<<endl;
            }
            if(!tb.next) break;
            ds.seekg((tb.next-1)*b+2*i);
            ds.read(cd(tb),b);
        }
        closestream();
    };
    int getcount(){
        return count;
    }
    /**/
    ~myblocklist(){}//不需要诶
};
template<typename T>
class baselog{
public:
    baselog()=default;
    void initial(string filen){
        filename=filen;
        file.open(filename,ios::in|ios::out|ios::binary);
        if(!file.is_open()){
            ofstream f1;
            f1.open(filename,ios::out|ios::binary);
            f1.seekp(0);
            int zero=0;
            f1.write(cd(zero),sizeof(int));
            f1.close();
        }
        else{
            file.seekg(0);
            file.read(cd(size),sizeof(int));
            T temp;
            for(int i=0;i<size;i++)
                file.read(cd(temp),sizeof(T)),store.push_back(temp);
        }
        file.close();
    }
    void insert(T data){
        file.open(filename,ios::in|ios::out|ios::binary);
        file.seekp(sizeof(int)+sizeof(T)*size);
        file.write(cd(data),sizeof(T));
        store.push_back(data);
        size++;
        file.seekp(0);
        file.write(cd(size),sizeof(int));
        file.close();
    }
    ~baselog(){}
protected:
    vector<T> store;
    fstream file;
    string filename;
    int size;
};
class finance: protected baselog<double>{
public:
    finance()=default;
    void initialf(string finance){
        initial(finance);
    }
    void insertf(double trade){
        insert(trade);
    }
    void show(int num){
        income=0;
        outcome=0;
        if(num>size) throw Error();
        for(int i=0;i<num;i++){
            if(store[size-1-i]<0)
                outcome-=store[size-1-i];
            if(store[size-1-i]>0)
                income+=store[size-1-i];
        }
        cout<<'+'<<' '<<fixed<<setprecision(2)<<income<<' '<<'-'<<' '<<outcome<<endl;
    }
    int getsize(){
        return size;
    }
    ~finance(){}
protected:
    double income=0;
    double outcome=0;
};
#endif //UNTITLED4_BOOKSTORE_H