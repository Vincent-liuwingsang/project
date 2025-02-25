#include<NTL/ZZ.h>
#include<NTL/LLL.h>
#include<NTL/mat_ZZ.h>
#include<NTL/RR.h>
#include<fstream>
#include<unordered_map>
#include<tuple>
#include<time.h>
#include<NTL/vec_RR.h>
#include<queue>
#include<cmath>
#include<list>
#include<string>
using namespace std;
using namespace NTL;

typedef tuple<vec_ZZ, ZZ> vector_and_ls;

void print_matrix(Mat<ZZ>& M);
void print_vector(vec_ZZ v);
void generate_random_p_vector(vec_ZZ& v, long n, long bit, ZZ seed);
void generate_random_gh_matrix(mat_ZZ& M, int n);
ZZ shortest_vector_found(mat_ZZ& M, int& i);
RR longest_vector_found(mat_ZZ& M);

//Gaussseive
bool GSieve(mat_ZZ& M, ZZ goal_square);
ZZ length_of_vector(vec_ZZ v);
vec_ZZ Ksample(mat_ZZ B, mat_RR mu, vec_RR c, vec_RR sps);
RR samplez(RR c, RR square,RR ln);
ZZ update(vec_ZZ& c,list<vector_and_ls>& L, queue<vec_ZZ>& S);
bool reduce(vec_ZZ& v1, vec_ZZ& v2);

int main(){

srand(time(NULL));

for (int n=30;n<=70;n+=2){
mat_ZZ B,B1,B2,B3;
B.SetDims(n,n);
generate_random_gh_matrix(B,n);
B1=B;
B2=B;
B3=B;
LLL_RR(B1);
BKZ_RR(B2);
int index;

//calculate goal square norm
double gamma = tgamma((double)n/2+1);
gamma=pow(gamma,1/(double)n);
RR gamma_r;
gamma_r = conv<RR>(gamma);
ZZ d;
d=abs(determinant(B1));
RR d_r;
d_r = conv<RR>(d);

RR ct;
ct=conv<RR>(1/(double)n);
d_r = pow(d_r,ct);
ct=conv<RR>(0.5);

RR gg;
gg=1.05*gamma_r*d_r/pow(M_PI,0.5);
gg*=gg;
ZZ goal_square;
goal_square=conv<ZZ>(gg);

//run experiments

clock_t start,end;
vec_ZZ x;
cout << "---------------------------------------------------------------Dimension " << n << "--------------------------------------------------------------------------------------------"<<endl;
cout << "target norm: " << SqrRoot(goal_square) << endl;
start=clock();
LLL_RR(B1);
end=clock();
ZZ shortest;
shortest=SqrRoot(shortest_vector_found(B1,index));
cout << "LLL: ";
if (LatticeSolve(x,B,B1[index])==1){
        cout << shortest;
}
else {
        cout << shortest << "found but not a lattice vector" << endl;
        print_vector(B1[index]);
}
cout << " in time " << (end-start)/(double)CLOCKS_PER_SEC*1000 << " ms" << endl;

start=clock();
BKZ_RR(B2);
end=clock();
shortest=SqrRoot(shortest_vector_found(B2,index));
cout << "BKZ: ";
if (LatticeSolve(x,B,B2[index])==1){
        cout << shortest;
}
else {
        cout << shortest << "found but not a lattice vector" << endl;
        print_vector(B2[index]);
}
cout << " in time " << (end-start)/(double)CLOCKS_PER_SEC*1000 << " ms" << endl;


start=clock();
BKZ_RR(B3,0.99,n,15);
end=clock();
shortest=SqrRoot(shortest_vector_found(B3,index));
cout << "BKZ(prune+max_size): ";
if (LatticeSolve(x,B,B3[index])==1){
        cout << shortest;
}
else {
        cout << shortest << "found but not a lattice vector" << endl;
        print_vector(B3[index]);
}
cout << " in time " << (end-start)/(double)CLOCKS_PER_SEC*1000 << " ms" << endl;


cout << "Gsieve(LLL): ";
start=clock();
if (GSieve(B1,goal_square)){
        end=clock();
        shortest=SqrRoot(shortest_vector_found(B1,index));
        vec_ZZ x;
        if (LatticeSolve(x,B,B1[index])==1){
                cout << shortest;
       }
        else {
                end=clock();
                cout << shortest << "found but not a lattice vector" << endl;
                print_vector(B1[index]);
}
}
else {
	cout << "failed";
}
cout << " in time " << (end-start)/(double)CLOCKS_PER_SEC*1000 << " ms" << endl;

cout << "Gsieve(BKZ): ";
start=clock();
if (GSieve(B2,goal_square)){
        end=clock();
        shortest=SqrRoot(shortest_vector_found(B2,index));
        vec_ZZ x;
        if (LatticeSolve(x,B,B2[index])==1){
                cout << shortest;
       }
        else {
                end=clock();
                cout << shortest << "found but not a lattice vector" << endl;
                print_vector(B2[index]);
}
}
else {
	cout << "failed";
}
cout << " in time " << (end-start)/(double)CLOCKS_PER_SEC*1000 << " ms" << endl;

cout << "Gsieve(BKZ(prune+max_size)): ";
start=clock();
if (GSieve(B3,goal_square)){
        end=clock();
        shortest=SqrRoot(shortest_vector_found(B3,index));
        vec_ZZ x;
        if (LatticeSolve(x,B,B3[index])==1){
                cout << shortest;
       }
        else {
                end=clock();
                cout << shortest << "found but not a lattice vector" << endl;
                print_vector(B3[index]);
}
}
else {
	cout << "failed";
}
cout << " in time " << (end-start)/(double)CLOCKS_PER_SEC*1000 << " ms" << endl;

}

/*
string file_name;
file_name="seed0-xx@.txt";

for (int n=40;n<=70;n+=2){
mat_ZZ B,B2;

B.SetDims(n,n);

string s = to_string(n);
for (int y=0;y<s.length();y++){
        file_name[y+6]=s[y];
}
cout << "Reading from file " << file_name << "......................................................................................................................................" << endl;
ifstream input(file_name);
if (input.is_open()){
	for (int i=0;i<n;i++){
		for (int j=0;j<n;j++){
                        input >> B[i][j];
		}
	}
	input.close();
}


B2=B;



//calculate goal square norm
double gamma = tgamma((double)n/2+1);
gamma=pow(gamma,1/(double)n);
RR gamma_r;
gamma_r = conv<RR>(gamma);
ZZ d;
d=abs(determinant(B2));
RR d_r;
d_r = conv<RR>(d);

RR ct;
ct=conv<RR>(1/(double)n);
d_r = pow(d_r,ct);
ct=conv<RR>(0.5);

RR gg;
gg=1.05*gamma_r*d_r/pow(M_PI,0.5);
gg*=gg;
ZZ goal_square;
goal_square=conv<ZZ>(gg);

//run experiments
int index;
vec_ZZ x;
cout << "---------------------------------------------------------------Dimension " << n << "--------------------------------------------------------------------------------------------"<<endl;
cout << "target norm: " << SqrRoot(goal_square) << endl;

clock_t start,end;
start=clock();
BKZ_RR(B2,0.99,n,15);
ZZ shortest;
shortest=SqrRoot(shortest_vector_found(B2,index));
cout << "BKZ: ";
if (LatticeSolve(x,B,B2[index])==1){
        cout << shortest << endl;
}
else {
        cout << shortest << "found but not a lattice vector" << endl;
        print_vector(B2[index]);
}

cout << "Gsieve: ";
if (GSieve(B2,shortest)){
//if (GSieve(B2,goal_square)){
        end=clock();
        shortest=SqrRoot(shortest_vector_found(B2,index));
        vec_ZZ x;
        if (LatticeSolve(x,B,B2[index])==1){
                cout << shortest << endl;
       }
        else {
                cout << shortest << "found but not a lattice vector" << endl;
                print_vector(B2[index]);
}
}
else {
	cout << "failed" << endl;
}

cout << "Total running time: " << (end-start)/(double)CLOCKS_PER_SEC*1000 << " ms" << endl;
}
*/
return 0;
}


void print_matrix(Mat<ZZ>& M){
for (int i=0;i<M.NumRows();i++){
	for (int j=0;j<M.NumCols();j++){
		cout << M[i][j] << " ";
	}
	cout << endl;
}
}

void print_vector(vec_ZZ v){
for (int i=0;i<v.length();i++){
cout << v[i] << " ";
}
cout << endl;
}

void generate_random_p_vector(vec_ZZ& v, long n, long bit, ZZ seed){
SetSeed(seed);
v.SetLength(n);
clear(v);
ZZ p;
GenPrime(p,bit*n);
v[0]=p;
for (int i=1;i<n;i++){
	RandomBnd(v[i],p);
	}
}

void generate_random_gh_matrix(mat_ZZ& M, int n){
vec_ZZ v;
ZZ seed;
RandomBnd(seed,(ZZ)100);
generate_random_p_vector(v,n,10,seed);
//generate_random_p_vector(v,n,10,(ZZ)0);

M[0][0]=v[0];
for (int i=1;i<n;i++){
	for (int j=1;j<n;j++){
		M[i][0]=v[i];
		M[i][i]=1;
	}
}
}

ZZ shortest_vector_found(mat_ZZ& M, int& j){
ZZ length_square;
InnerProduct(length_square,M[0],M[0]);
j=0;
for (int i=1;i<M.NumRows();i++){
	ZZ length;
	InnerProduct(length,M[i],M[i]);
	if (length<length_square){
		length_square=length;
                j=i;
	}
}
return length_square;
}

RR longest_vector_found(mat_ZZ& M){
ZZ length_square;
InnerProduct(length_square,M[0],M[0]);
for (int i=1;i<M.NumRows();i++){
	ZZ length;
	InnerProduct(length,M[i],M[i]);
	if (length>length_square){
		length_square=length;
	}
}

RR length;
length = conv<RR>(length_square);
length=SqrRoot(length);
return length;
}


//   GaussSieve


bool GSieve(mat_ZZ& M, ZZ goal_square){
int n=M.NumRows();
int index;
ZZ min;
min = shortest_vector_found(M,index);
vec_ZZ min_vector;
min_vector = M[index];
ZZ collisions ;
ZZ max_list_size ;

list<vector_and_ls> L;
queue<vec_ZZ> S;

// for KleinSampling
mat_RR mu;
vec_RR c,sps;
sps.SetLength(n);
c.SetLength(n);

ComputeGS(M,mu,c);

RR max_c;
for (int i=0;i<n;i++){
	if (c[i]>max_c){
	max_c=c[i];
	}
}

RR ssquare;
RR t;
conv(t,log(n));

ssquare=max_c*t;
for (int i=0;i<n;i++){
	sps[i]=ssquare/c[i];
}



//update
for (int i=0;i<n;i++){
	update(M[i],L,S);
}
max_list_size=L.size();
//update max_list_size
while ((min>goal_square) && (collisions<(ZZ)(0.1*max_list_size+(ZZ)200))){
//cout << min << ">" << goal_square << "              " << collisions << "< " <<(0.1*max_list_size+(ZZ)200) << endl;
	vec_ZZ current;

        if (max_list_size < (ZZ)L.size()){
                conv(max_list_size,L.size());
        }
  //      cout << "here0" << endl;

        if (!S.empty()){
		current=S.front();
		S.pop();
	}
	else{
		current=Ksample(M,mu,c,sps);
	}
	/*cout << "current vector: [";
	for (int i=0;i<n;i++){
		cout << " " << current[i];
	}
	cout << "]" << endl;
	*/
	//GaussReduce
//        cout << "here1" << endl;
	ZZ cns;
	cns=update(current,L,S);

//        cout << "here2" << endl;
	if(cns==(ZZ)0){
		collisions++;
	}
	if (cns>(ZZ)0 && cns<min){
		min=cns;
		min_vector = current;
	}
}
if (min>goal_square){
	return false;
}
M[0]=min_vector;
return true;
}


ZZ length_of_vector(vec_ZZ v){
int n=v.length();
ZZ temp;
for (int i=0;i<n;i++){
	temp+=v[i]*v[i];
}
return temp;
}


vec_ZZ Ksample(mat_ZZ B, mat_RR mu, vec_RR c, vec_RR sps){//check address
long n=B.NumRows();
double ln=log(n);
RR lnn;
conv(lnn,ln);
vec_RR temp;
temp.SetLength(n);


for (int i=n-1;i>=0;--i){
	temp[i]=samplez(temp[i], sps[i], lnn);
	for (int j=0;j<i;j++){
		temp[j] -= (temp[i]*mu[i][j]);
	}
}
//print_vector(temp);
vec_RR v;
v.SetLength(n);
for (int i=0;i<n;i++){
for (int j=0;j<n;j++){
	RR tt;
	conv(tt,B[j][i]);
	v[i]+=(RR)temp[j]*tt;
}
}
vec_ZZ u;
conv(u,v);
return u;

}

RR samplez(RR c, RR square,RR ln){
RR s;
s=SqrRoot(square);
RR min_c,max_c;
min_c = floor(c-s*ln);
max_c = ceil(c+s*ln);
RR x,rho;
while(true){
	x=min_c+round((max_c-min_c)*(RR)rand()/RAND_MAX);
	const RR r=-M_PI*(x-c)*(x-c)/square;
	rho=exp(r);
	if ((RR)rand()/RAND_MAX<=rho){//check
		return x;
	}
}
}


ZZ update(vec_ZZ& c,list<vector_and_ls>& L, queue<vec_ZZ>& S){
bool reduced = false;
list<vector_and_ls>::iterator it,tit;
vec_ZZ v;
ZZ ls;

while (!reduced){
	reduced = true;
	for (it=L.begin(); it!=L.end(); it++){
		tie(v,ls)=*it;
                //cout << ls << "<=" <<length_of_vector(c)<<endl;;
		if (ls <= length_of_vector(c)){
			if (reduce(c,v)){
				reduced=false;
			}
		}
	}
}
ZZ length_of_c;
length_of_c = length_of_vector(c);

if (length_of_c==0){
	//cout << "collisions";
	return (ZZ)0;
}
vector_and_ls gggg;
gggg=make_tuple(c,length_of_c);
L.insert(it,gggg);
while (it!=L.end()){
	tit=it;
	vec_ZZ temp_v;
	ZZ temp_ls;
	tie(temp_v,temp_ls)=*it;
	if (reduce(temp_v,c)){
		L.erase(tit);
		S.push(c);
	}
	it++;
}

return length_of_c;
}

bool reduce(vec_ZZ& v1, vec_ZZ& v2){
long n=v1.length();
ZZ ip;
ip=v1*v2;
//cout << abs(ip) << "<=" <<length_of_vector(v2) << endl;
if (abs((ZZ)2*ip)<=length_of_vector(v2)){
	return false;
}
RR ip_RR,ls_RR;
ip_RR=conv<RR>(ip);
ls_RR=conv<RR>(length_of_vector(v2));

RR r = round(ip_RR/ls_RR);
ZZ q;
conv(q,r);
for (long i=0;i<n;i++){
	v1[i]-=v2[i]*q;
}
return true;
}
