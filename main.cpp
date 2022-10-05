//************************//
//***** HEADER FILES *****//
//************************//
#include<iostream>
#include<cstring>
#include<string.h>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<conio.h>
#include<windows.h>
#include<string>

using namespace std;

//*********************************//
//***** GLOBAL VARIABLES USED *****//
//*********************************//
using std::setw;
string user="admin", assistant="lab_assistant";
string passcode="ml123", assistant_pass="la123";
int size=0;

//*******************************//
//***** REQUIRED STRUCTURES *****//
//*******************************//
struct Equipment {
	string _id;
    string description;
    string dop;
    int quantity;
    float amount;
    string grant;
    string purchaser;
    string pur_details;
    bool status;
    string sign;
    string remark;
    int date;
};

struct All_equipments {
    Equipment eqps[1000];
};

//Declarations
void display_single(All_equipments &All, int i, int allowed);
void display_all(All_equipments &All);

//****************************************************************//
//***** FUNCTIONS FOR PERFORMING VALIDATIONS ON INPUT FIELDS *****//
//****************************************************************//
//auto generate id
string generate_id(){
	string id,str; 
  	stringstream ss;  
  	ss << size;  
  	ss >> str;
	if(size/10==0){
		id="ML00"+str;
	}else if(size/100==0){
		id="ML0"+str;
	}else{
		id="ML"+str;
	}
	return id;
}
//validate date format
string validate_date(string dop){
	while(true){
		while(dop.length()!=10){
			cout<<setw(45)<<"Invalid date format. Plz use DD/MM/YYYY "<<" : \t";
    		std::getline(cin, dop);
		}
		string d, m, y, s1, s2;
		int DD,MM,Y;
		d=dop.substr(0,2);
		m=dop.substr(3,4);
		y=dop.substr(6,10);
		s1=dop[2];
		s2=dop[5];
		stringstream mk(d);
		mk>>DD;
		stringstream mm(m);
		mm>>MM;
		stringstream my(y);
		my>>Y;
		if(MM>0 && MM<13 && (s1=="/" && s2=="/")){
			if((MM==1 || MM==3 || MM==5 || MM==7 || MM==8 || MM==10 || MM==12) && (DD>0 && DD<32)){break;
			}else if((MM==4 || MM==6 || MM==9 || MM==11) && (DD>0 && DD<31)){break;
			}else if(MM==2 && Y%4==0 && DD>0 && DD<30){break;
			}else if(MM==2 && Y%4!=0 && DD>0 && DD<29){break;
			}else{
				cout<<setw(45)<<"Invalid Date format. Plz use DD/MM/YYYY "<<" : \t";
				std::getline(cin, dop);
			}
		}else{
			cout<<setw(45)<<"Invalid Date format. Plz use DD/MM/YYYY "<<" : \t";
			std::getline(cin, dop);
		}
	}
	return dop;
}
//validate non-negative input fields
int validate_non_neg(string q){
	while(true){
		if(q==""){
			cout<<setw(45)<<"Quantity is required "<<" : \t";
			getline(cin, q);
			continue;
		}
		int num, all_digit=1;
		for(int i=0; i<q.length(); i++){
			if(!(q[i]>='0' && q[i]<='9')){
				all_digit=0;
				break;
			}
		}
		if(all_digit==1){
			stringstream mk(q);
			mk>>num;
			if(num<0){
				cout<<setw(45)<<"Invalid input. Enter a positive integer "<<" : \t";
				getline(cin, q);
			}else{
				return num;	
			}	
		}else{
			cout<<setw(45)<<"Invalid input. Enter a positive integer "<<" : \t";
			getline(cin, q);
		}	
	}
}
//validate positive float value with onlt 2 decimals
float validate_amount(string q){
	while(true){
		float all_digit=1;
		for(int i=0; i<q.length(); i++){
			if(!((q[i]>='0' && q[i]<='9') || q[i]=='.')){
				all_digit=0;
				break;
			}
		}
		if(all_digit==1){
			double num = ::atof(q.c_str());
			int v = num*100;
			double k = v/100.0;
			if(num<0 || k!=num){
				cout<<setw(45)<<"Invalid input. Enter a positive float value "<<" : \t";
				getline(cin, q);
			}else{
				return num;	
			}	
		}else{
			cout<<setw(45)<<"Invalid input. Enter a positive float value "<<" : \t";
			getline(cin, q);
		}	
	}
}
//validate status field
void validate_status(string num, Equipment &E){
	while(true){
    	if(num=="1"){
    		E.status=1;
    		break;
		}else if(num=="0"){
			E.status=0;
			break;
		}else{
			cout<<setw(45)<<"Invalid input please try again "<<" : \t";
			getline(cin, num);
		}
	}
}
//validate required flieds
string validate_required(string s){
	while(s=="" || !(s.find_first_not_of(' ')!=std::string::npos)){
		cout<<setw(45)<<"Description is required "<<" : \t";
		getline(cin, s);
	}
	return s;
}

//*************************************//
//***** FUNCTIONS FOR CALCULATION *****//
//*************************************//
//Amount of Available Elements//
float amount_avail(All_equipments &All){
    float amount=0;
    for(int i=0; i<size; i++){
        if(All.eqps[i].status){
            amount=amount+All.eqps[i].amount;
        }
    }
    return amount;
}
//Amount of Not-Available Elements//
float amount_n_avail(All_equipments &All){
    float amount=0;
    for(int i=0; i<size; i++){
        if(!All.eqps[i].status){
        	amount=amount+All.eqps[i].amount;
        }
    }
    return amount;
}
int convert_date(string date){
    string d, m, y;
    int DD,MM,Y, date_int;
    d=date.substr(0,2);
    m=date.substr(3,4);
    y=date.substr(6,10);
    stringstream mk(d);
    mk>>DD;
    stringstream mm(m);
    mm>>MM;
    stringstream my(y);
    my>>Y;
    date_int = Y*10000 + MM*100 + DD;
    return date_int;
}
void int_date(All_equipments &All){
	for(int i=0; i<size; i++){
		string d, m, y;
		int DD,MM,Y, fdate;
		d=All.eqps[i].dop.substr(0,2);
		m=All.eqps[i].dop.substr(3,4);
		y=All.eqps[i].dop.substr(6,10);
		stringstream mk(d);
		mk>>DD;
		stringstream mm(m);
		mm>>MM;
		stringstream my(y);
		my>>Y;
		fdate = Y*10000 + MM*100 + DD;
		All.eqps[i].date=fdate;
	}
}

//**************************************************//
//***** FUNCTIONS FOR TAKING INPUTS FROM USER  *****//
//**************************************************//
//Function to read details of an equipment
void read_details(Equipment &E, All_equipments &All){
	string num, status;
	E._id=generate_id();
	cout<<endl<<setw(45)<<"Id "<<" : \t"<<E._id<<endl;
    cout<<setw(45)<<"Description* "<<" : \t";
    cin.ignore();
    getline(cin,num);
    E.description=validate_required(num);
    cout<<setw(45)<<"Date of purchase(DD/MM/YYYY)* "<<" : \t";
	getline(cin, E.dop);
    E.dop=validate_date(E.dop);
    cout<<setw(45)<<"Quantity* "<<" : \t";
    getline(cin, num);	
    E.quantity=validate_non_neg(num);
    cout<<setw(45)<<"Amount "<<" : \t";
    getline(cin, num);
    E.amount=validate_amount(num);
    cout<<setw(45)<<"Grant "<<" : \t";
	getline(cin, E.grant);
    cout<<setw(45)<<"Purchaser "<<" : \t";  
    getline(cin, E.purchaser);
    cout<<setw(45)<<"Purchase details "<<" : \t";
    getline(cin, E.pur_details);
    cout<<setw(45)<<"Status (1: Available; 0: Not Available)* "<<" : \t";
    getline(cin, status);
    validate_status(status, E);
    cout<<setw(45)<<"Sign of lab coordinator "<<" : \t";
    getline(cin, E.sign);
    cout<<setw(45)<<"Remarks "<<" : \t";
    getline(cin, E.remark);
}
//Function to add a single equipment
void add2array(All_equipments &All){
	size=size+1;
	read_details(All.eqps[size-1], All);
}
//Function to add 'n' number of equipments
void add_n_equips(All_equipments &All){
	int n;
	cout<<"\n\n\t"<<string(100, '_')<<endl;
	cout<<"\n\t"<<setw(58)<<"A D D   D A T A"<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<"\n\tEnter the number of equipments you want to add: ";
	cin>>n;
	for(int i=0; i<n; i++){
		cout<<"\t"<<string(100, '_')<<endl;
		add2array(All);
		cout<<endl;
	}
	cout<<"\t"<<string(100, '_')<<endl;
}

//********************************//
//***** FUNCTIONS FOR SEARCH *****//
//********************************//
//Function to search and display equipment by id
int search_by_id(All_equipments &All, int allowed){
	char ID[10];
	system("cls");
	cout<<"\n\n\t"<<string(100, '_')<<endl;
	cout<<endl<<setw(80)<<"S E A R C H   T H R O U G H   D A T A   B Y   I D"<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<"\n\tEnter the ID of the equipment to be searched : ";
	cin.ignore();
	cin.getline(ID, 10);
	cout<<"\t"<<string(100, '_')<<endl;
	if(strlen(ID)!=5){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		cout<<"\n\tOOPS!!! Equipment with id="<<ID<<" is not present in the record.";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
		cout<<endl<<"\t"<<string(100, '_')<<endl;
		return 0;
	}
	for(int i=0; i<size; i++){
		if(ID==All.eqps[i]._id){
			display_single(All, i, allowed);
			cout<<"\t"<<string(100, '_')<<endl;
			return i;
		}
	}
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	cout<<"\n\tOOPS!!! Equiment with id="<<ID<<" is not present in the record."<<endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
	cout<<"\t"<<string(100, '_')<<endl;
	return 0;
}
//Function to search and display equipment by description
void search_by_description(All_equipments &All, int allowed){
	string str;
	system("cls");
	int cnt=0;
	cout<<"\n\n\t"<<string(100, '_')<<endl;
	cout<<endl<<setw(85)<<"S E A R C H   T H R O U G H   D A T A   B Y   D E S C R I P T I O N"<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<"\n\tEnter some keywords to find the equipments : ";
	cin.ignore();
	getline(cin, str);
	cout<<"\t"<<string(100, '_')<<endl;
	for(int i=0; i<size; i++){
		int index,pos=0;
		while((index = All.eqps[i].description.find(str,pos)) != string::npos){
			display_single(All, i, allowed);
			cout<<"\t"<<string(100, '_')<<endl;
			cnt=cnt+1;
			pos=index+1;
		}
	}
	if(cnt==0){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		cout<<"\n\tOOPS!!! No matching results found!!!"<<endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
		cout<<"\t"<<string(100, '_')<<endl;
	}
}

//****************************************//
//***** FUNCTIONS FOR MODIFYING DATA *****//
//****************************************//
void modify(All_equipments &All){
	string id, str="";
	int k=0;
	string date;
	cout<<"\t"<<string(100, '_')<<endl<<endl;
	cout<<setw(70)<<"M O D I F Y   D A T A"<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<"\n\tEnter the id of equipment to be modified : ";
	getline(cin, id);
	cout<<"\t"<<string(100, '_')<<endl;
	for(int i=0; i<size; i++){
		if(id==All.eqps[i]._id){
			cout<<"\tEXISTING INFORMATION : "<<endl;
			display_single(All,i, 1);
			cout<<setw(60)<<string(30, '_')<<endl;
			cout<<endl<<"\tENTER THE MODIFICATIONS : \n\t  Note: If you don't want to modify a particular field then press ENTER"<<endl;
			cout<<setw(45)<<"Id "<<" : \t"<<All.eqps[i]._id<<endl;
			cout<<setw(45)<<"Description "<<" : \t";
			getline(cin, str);
			if(str!=""){
				All.eqps[i].description=str;
			}
			cout<<setw(45)<<"Date of purchase(DD/MM/YYYY) "<<" : \t";
			getline(cin, date);
			if(date!=""){
				All.eqps[i].dop=validate_date(date);
			}
			cout<<setw(45)<<"Quantity "<<" : \t";
   		 	getline(cin, str);
   		 	if(str!=""){
   		 		All.eqps[i].quantity=validate_non_neg(str);	
			}
    		cout<<setw(45)<<"Amount "<<" : \t";
    		getline(cin, str);
   		 	if(str!=""){
   		 		All.eqps[i].amount=validate_amount(str);	
			}
			cout<<setw(45)<<"Grant "<<" : \t";
			getline(cin, str);
			if(str!=""){
				All.eqps[i].grant=str;
			}
			cout<<setw(45)<<"Purchaser "<<" : \t";
			getline(cin, str);
			if(str!=""){
				All.eqps[i].purchaser=str;
			}
			cout<<setw(45)<<"Purchase Details "<<" : \t";
			getline(cin, str);
			if(str!=""){
				All.eqps[i].pur_details=str;
			}
			cout<<setw(45)<<"Status (1: Available; 0: Not Available) "<<" : \t";
			getline(cin, str);
			if(str!=""){
				validate_status(str, All.eqps[i]);
			}
			cout<<setw(45)<<"Sign of lab coordinator "<<" : \t";
			getline(cin, str);
			if(str!=""){
				All.eqps[i].sign=str;
			}
			cout<<setw(45)<<"Remarks "<<" : \t";
			getline(cin, str);
			if(str!=""){
				All.eqps[i].remark=str;
			}
			cout<<"\t"<<string(100, '_')<<endl;
			cout<<"\n\tModifying the data";
            for(int j=0; j<7; j++){
                cout<<".";  Sleep(500);
            }
			k=1;
			break;
		}
	}
	if(k==0){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		cout<<endl<<"\tOOPS!!! Equipment with id="<<id<<" is not present in the data.";
        Sleep(1500);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
	}
}

//****************************************//
//*****  FUNCTIONS FOR SORTING DATA  *****//
//****************************************//
All_equipments sortData(All_equipments &All){
	All_equipments Sorted;
	Sorted = All;
	int_date(Sorted);
	int i, j , min;
	Equipment temp;
	for(i=0; i<size-1; i++){
		min=i;
		for(j=i+1; j<size; j++){
			if(Sorted.eqps[j].date < Sorted.eqps[min].date){
				min=j;
				temp=Sorted.eqps[i];
				Sorted.eqps[i]=Sorted.eqps[min];
				Sorted.eqps[min]=temp;
			}
		}
	}
	return Sorted;
}

//*****************************************************//
//***** FUNCTIONS TO OUTPUT THE RECORDS ON CONSOLE*****//
//*****************************************************//
//Display single element
void display_single(All_equipments &All, int i, int allowed){
	cout<<setw(45)<<"Id "<<" : \t"<<All.eqps[i]._id<<endl;
   	cout<<setw(45)<<"Description "<<" : \t"<<All.eqps[i].description<<endl;
    if(allowed){
        cout<<setw(45)<<"Date of purchase(DD/MM/YYYY) "<<" : \t"<<All.eqps[i].dop<<endl;
    }
   	cout<<setw(45)<<"Quantity "<<" : \t"<<All.eqps[i].quantity<<endl;
    if(allowed){
        cout<<setw(45)<<"Amount "<<" : \t"<<All.eqps[i].amount;
        if(All.eqps[i].amount!=0){
            cout<<"/-";	
        }
        cout<<endl;
    }
    if(allowed){
        cout<<setw(45)<<"Grant "<<" : \t"<<All.eqps[i].grant<<endl;
        cout<<setw(45)<<"Purchaser "<<" : \t"<<All.eqps[i].purchaser<<endl;  
        cout<<setw(45)<<"Purchase details "<<" : \t"<<All.eqps[i].pur_details<<endl;
    }
   	cout<<setw(45)<<"Status "<<" : \t"<<All.eqps[i].status<<endl;
    if(allowed){
        cout<<setw(45)<<"Sign of lab coordinator "<<" : \t"<<All.eqps[i].sign<<endl;
        cout<<setw(45)<<"Remarks "<<" : \t"<<All.eqps[i].remark<<endl;
    }
}
//Display all the equipments existing in record
void display_all(All_equipments &All, int allowed){
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<endl<<"\t"<<setw(72)<<"D I S P L A Y I N G   T H E   E N T I R E   D A T A"<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	for(int i=0; i<size; i++){
		display_single(All, i, allowed);
		cout<<"\t"<<string(100, '_')<<endl;
		Sleep(400);
	}
}
//Display available equipments
void display_avail(All_equipments &All, int allowed){
    cout<<"\t"<<string(100, '_')<<endl;
	cout<<endl<<"\t"<<setw(77)<<"D I S P L A Y I N G   T H E   A V A I L A B L E   E Q U I P M E N T S"<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	for(int i=0; i<size; i++){
        if(All.eqps[i].status){
            display_single(All, i, allowed);
            cout<<"\t"<<string(100, '_')<<endl;
        }
		Sleep(400);
	}
	cout<<"\t"<<string(100, '_')<<endl;
}
//Display NOT available equipments
void display_n_avail(All_equipments &All, int allowed){
    cout<<"\t"<<string(100, '_')<<endl;
	cout<<endl<<"\t"<<setw(78)<<"D I S P L A Y I N G   T H E   N O T - A V A I L A B L E   E Q U I P M E N T S"<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	for(int i=0; i<size; i++){
        if(!All.eqps[i].status){
            display_single(All, i, allowed);
        	cout<<"\t"<<string(100, '_')<<endl;
		}
		Sleep(400);
	}
	cout<<"\t"<<string(100, '_')<<endl;
}
//Display amount statistics
void display_statistics(All_equipments &All){
	float amt_avail, amt_navail, amt_tot;
	cout<<string(11, '\n')<<"\t"<<string(100, '_')<<endl;
	cout<<endl<<"\t"<<setw(82)<<"D I S P L A Y I N G   T H E   A M O U N T   S T A T I S T I C S"<<endl;
	cout<<"\t"<<string(100, '_')<<endl<<endl<<endl;
	amt_avail=amount_avail(All);
	amt_navail=amount_n_avail(All);
	amt_tot=amt_avail+amt_navail;
	cout<<setw(65)<<"Amount spent on buying available equipments "<<" : "<<setw(15)<<amt_avail<<"/-"<<endl<<endl;
	cout<<setw(65)<<"Amount spent on buying unavailable equipments "<<" : "<<setw(15)<<amt_navail<<"/-"<<endl<<endl;
	cout<<setw(83)<<string(65, '_')<<endl<<endl;
	cout<<setw(65)<<"T O T A L "<<" : "<<setw(15)<<amt_tot<<"/-"<<endl<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
}
//Display equipment sorted according to date of purchase
void display_sorted(All_equipments &All){
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<endl<<"\t"<<setw(72)<<"D I S P L A Y I N G   T H E   S O R T E D   D A T A"<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	for(int i=0; i<size; i++){
		display_single(All, i, 1);
		cout<<"\t"<<string(100, '_')<<endl;
		Sleep(400);
	}
}
//Display sorted data over a period
void display_period(All_equipments &All){
    All_equipments Sorted;
    Sorted = sortData(All);
    int_date(Sorted);
    string date1, date2;
    cout<<"\t"<<string(100, '_')<<endl<<endl;
    cout<<setw(45)<<"Enter the start date "<<" : \t";
    cin.ignore();
    getline(cin, date1);
    date1=validate_date(date1);
    cout<<setw(45)<<"Enter the stop date "<<" : \t";
    getline(cin, date2);
    date2=validate_date(date2);
    int d1=convert_date(date1);
    int d2=convert_date(date2);
    cout<<"\t"<<string(100, '_')<<endl;
    cout<<endl<<setw(82)<<"D I S P L A Y I N G   E Q U I P M E N T S   P U R C H A S E D   F R O M   "<<date1<<"   TO   "<<date2<<endl;
    cout<<"\t"<<string(100, '_')<<endl;
    if(d1<d2){
        int cnt=0, amt=0;
        for(int i=0; i<size; i++){
            if(Sorted.eqps[i].date>=d1 && Sorted.eqps[i].date<=d2){
                display_single(Sorted, i, 1);
                amt += Sorted.eqps[i].amount;
                cnt++;
                cout<<"\t"<<string(100, '_')<<endl;
            }
            Sleep(400);
        }
        if(cnt==0){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
            cout<<endl<<"\t\t"<<"No Equipment was purchased between "<<date1<<" and "<<date2<<"."<<endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
            cout<<"\t"<<string(100, '_')<<endl;
        }else{
            cout<<endl<<"\t\t\t"<<string(60, '*')<<endl<<endl;
            cout<<setw(60)<<"Total Equipments bought : "<<setw(15)<<cnt<<endl;
            cout<<setw(60)<<"Total Amount spent : "<<setw(15)<<amt<<"/-"<<endl;
            cout<<endl<<"\t\t\t"<<string(60, '*')<<endl;
             cout<<"\t"<<string(100, '_')<<endl;
        }
    }else{
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
        cout<<endl<<"\t\t"<<"No Equipment was purchased between "<<date1<<" and "<<date2<<"."<<endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
        cout<<"\t"<<string(100, '_')<<endl;
    }
}

//*****************************************************//
//***** ADD THE ENTIRE EXISTING DATA TO .CSV FILE *****//
//*****************************************************//
void put_data(All_equipments &All){
	ofstream DataFile;
	DataFile.open("ml_data.csv");
	DataFile<<"ID,DESCRIPTION,DATE OF PURCHASE,QUANTITY,AMOUNT,GRANT,PURCHASER,PURCHASE DETAILS,STATUS,SIGN,REMARK"<<endl;
	for(int i=0; i<size; i++){
		DataFile<<All.eqps[i]._id<<","<<All.eqps[i].description<<","<<All.eqps[i].dop<<","<<All.eqps[i].quantity<<","<<
		All.eqps[i].amount<<","<<All.eqps[i].grant<<","<<All.eqps[i].purchaser<<","<<All.eqps[i].pur_details<<","<<
		All.eqps[i].status<<","<<All.eqps[i].sign<<","<<All.eqps[i].remark<<endl;
	}
}

//************************************//
//***** READ DATA FROM .CSV FILE *****//
//************************************//
void read_data(All_equipments &All){
	int cnt=0, num;
	std::ifstream  data("ml_data.csv");
	string id, description, dop, quantity, amount, grant, purchaser, details, status, sign, remark;
	while(data.good()){
		std::getline(data, id, ',');
		std::getline(data, description, ',');
		std::getline(data, dop, ',');
		std::getline(data, quantity, ',');
		std::getline(data, amount, ',');
		std::getline(data, grant, ',');
		std::getline(data, purchaser, ',');
		std::getline(data, details, ',');
		std::getline(data, status, ',');
		std::getline(data, sign, ',');
		std::getline(data, remark,'\n');		
		if(cnt!=0){
			All.eqps[cnt-1]._id=id;
			All.eqps[cnt-1].description=description;
			All.eqps[cnt-1].dop=dop;
			stringstream q(quantity);
			q>>num;
			All.eqps[cnt-1].quantity=num;
			stringstream a(amount);
			a>>num;
			All.eqps[cnt-1].amount=num;
			All.eqps[cnt-1].grant=grant;
			All.eqps[cnt-1].purchaser=purchaser;
			All.eqps[cnt-1].pur_details=details;
			stringstream s(status);
			s>>num;
			All.eqps[cnt-1].status=num;
			All.eqps[cnt-1].sign=sign;
			All.eqps[cnt-1].remark=remark;
		}
		cnt=cnt+1;
	}
	size=cnt-2;
}

//**************************//
//***** Login function *****//
//**************************//
int login(int chance){
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");
	char userName[100], ch;
	string pass="", pass1="";
	int  verification=0;
	cout<<string(11, '\n');
	cout<<endl<<"\t"<<string(100, '_')<<endl;
	cout<<endl<<setw(80)<<"W E L C O M E   T O   M A C H I N E S   L A B"<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<endl<<"\t"<<setw(60)<<"A D M I N   L O G I N"<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
    SetConsoleTextAttribute(h, FOREGROUND_RED);
	if(chance>1){
		cout<<endl<<"\t\t\t\tOOPS - Incorrect username or password!!! Try again!!!";
	}
    SetConsoleTextAttribute(h, 0x07);
	cout<<endl<<"\t\t\t\tPlease enter the user name and password"<<endl<<endl;
	cout<<setw(45)<<"User Name "<<" : \t";
	cin.getline(userName, 100);
	cout<<endl<<setw(45)<<"Password "<<" : \t";
	ch = _getch();
   	while(ch != 13){
   	  if(ch==8){
   	  	int l=pass1.length();
   	  	l=l-1;
   	  	pass=pass.substr(0,l);
   	  	pass1 = pass1.substr(0,l);
   	  	system("cls");
   	  	cout<<string(11, '\n');
   	  	cout<<endl<<"\t"<<string(100, '_')<<endl;
		cout<<endl<<setw(80)<<"W E L C O M E   T O   M A C H I N E S   L A B"<<endl;
		cout<<"\t"<<string(100, '_')<<endl;
		cout<<endl<<"\t"<<setw(60)<<"A D M I N   L O G I N"<<endl;
		cout<<"\t"<<string(100, '_')<<endl;
        SetConsoleTextAttribute(h, FOREGROUND_RED);
		if(chance>1){
			cout<<endl<<"\t\t\t\tOOPS - Incorrect username or password!!! Try again!!!";
		}
        SetConsoleTextAttribute(h, 0x07);
		cout<<endl<<"\t\t\t\tPlease enter the user name and password"<<endl<<endl;
		cout<<setw(45)<<"User Name "<<" : \t"<<userName;
		cout<<endl<<endl<<setw(45)<<"Password "<<" : \t"<<pass1;
	  }	  
	  if(ch!=8){
	   	pass.push_back(ch);
	  	pass1.append("*");
        system("cls");
        cout<<string(11, '\n');
        cout<<endl<<"\t"<<string(100, '_')<<endl;
		cout<<endl<<setw(80)<<"W E L C O M E   T O   M A C H I N E S   L A B"<<endl;
		cout<<"\t"<<string(100, '_')<<endl;
		cout<<endl<<"\t"<<setw(60)<<"A D M I N   L O G I N"<<endl;
		cout<<"\t"<<string(100, '_')<<endl;
        SetConsoleTextAttribute(h, FOREGROUND_RED);
		if(chance>1){
			cout<<endl<<"\t\t\t\tOOPS - Incorrect username or password!!! Try again!!!";
		}
        SetConsoleTextAttribute(h, 0x07);
		cout<<endl<<"\t\t\t\tPlease enter the user name and password"<<endl<<endl;
		cout<<setw(45)<<"User Name "<<" : \t"<<userName;
		cout<<endl<<endl<<setw(45)<<"Password "<<" : \t"<<pass1;
	  }
      ch = _getch();
   	}
   	cout<<endl;
	if(userName==user && pass==passcode){
		verification=1;
	}
	return verification;
}
int login_assistant(int chance){
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");
	char userName[100], ch;
	string pass="", pass1="";
	int  verification=0;
	cout<<string(11, '\n');
	cout<<endl<<"\t"<<string(100, '_')<<endl;
	cout<<endl<<setw(80)<<"W E L C O M E   T O   M A C H I N E S   L A B"<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<endl<<"\t"<<setw(70)<<"L A B   A S S I S T A N T   L O G I N"<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
    SetConsoleTextAttribute(h, FOREGROUND_RED);
	if(chance>1){
		cout<<endl<<"\t\t\t\tOOPS - Incorrect username or password!!! Try again!!!";
	}
    SetConsoleTextAttribute(h, 0x07);
	cout<<endl<<"\t\t\t\tPlease enter the user name and password"<<endl<<endl;
	cout<<setw(45)<<"User Name "<<" : \t";
	cin.getline(userName, 100);
	cout<<endl<<setw(45)<<"Password "<<" : \t";
	ch = _getch();
   	while(ch != 13){
   	  if(ch==8){
   	  	int l=pass1.length();
   	  	l=l-1;
   	  	pass=pass.substr(0,l);
   	  	pass1 = pass1.substr(0,l);
   	  	system("cls");
   	  	cout<<string(11, '\n');
   	  	cout<<endl<<"\t"<<string(100, '_')<<endl;
		cout<<endl<<setw(80)<<"W E L C O M E   T O   M A C H I N E S   L A B"<<endl;
		cout<<"\t"<<string(100, '_')<<endl;
		cout<<endl<<"\t"<<setw(70)<<"L A B   A S S I S T A N T   L O G I N"<<endl;
		cout<<"\t"<<string(100, '_')<<endl;
        SetConsoleTextAttribute(h, FOREGROUND_RED);
		if(chance>1){
			cout<<endl<<"\t\t\t\tOOPS - Incorrect username or password!!! Try again!!!";
		}
        SetConsoleTextAttribute(h, 0x07);
		cout<<endl<<"\t\t\t\tPlease enter the user name and password"<<endl<<endl;
		cout<<setw(45)<<"User Name "<<" : \t"<<userName;
		cout<<endl<<endl<<setw(45)<<"Password "<<" : \t"<<pass1;
	  }	  
	  if(ch!=8){
	   	pass.push_back(ch);
	  	pass1.append("*");
        system("cls");
        cout<<string(11, '\n');
        cout<<endl<<"\t"<<string(100, '_')<<endl;
		cout<<endl<<setw(80)<<"W E L C O M E   T O   M A C H I N E S   L A B"<<endl;
		cout<<"\t"<<string(100, '_')<<endl;
		cout<<endl<<"\t"<<setw(70)<<"L A B   A S S I S T A N T   L O G I N"<<endl;
		cout<<"\t"<<string(100, '_')<<endl;
        SetConsoleTextAttribute(h, FOREGROUND_RED);
		if(chance>1){
			cout<<endl<<"\t\t\t\tOOPS - Incorrect username or password!!! Try again!!!";
		}
        SetConsoleTextAttribute(h, 0x07);
		cout<<endl<<"\t\t\t\tPlease enter the user name and password"<<endl<<endl;
		cout<<setw(45)<<"User Name "<<" : \t"<<userName;
		cout<<endl<<endl<<setw(45)<<"Password "<<" : \t"<<pass1;
	  }
      ch = _getch();
   	}
   	cout<<endl;
	if(userName==assistant && pass==assistant_pass){
		verification=1;
	}
	return verification;
}
//**********************************//
//***** INSTRUCTIONS FUNCTIONS *****//
//**********************************//
char login_instructions(){
	char s;
	cout<<string(11, '\n');
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<"\n\t"<<setw(70)<<"W E L C O M E   TO   M A C H I N E S   L A B"<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<"\n\t\t Types of Logins available:"<<endl;
	cout<<"\t\t"<<string(40, '_')<<endl;
	cout<<"\n\t\tA: Login as Admin."<<endl;
	cout<<"\t\tL: Login as Lab Assistant."<<endl;
    cout<<"\t\tS: Login as Student."<<endl;
	cout<<"\t\tE: Close the Application"<<endl<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<"\n\t\tCHOOSE THE OPERATION THAT YOU WANT TO PERFORM : ";
	cin>>s;
	cout<<endl;
	return s;
}
char instructions(int allowed, int student){
	char ch;
	system("cls");
	cout<<string(11, '\n');
    if(student){
        cout<<"\t"<<string(100, '_')<<endl;
        cout<<"\n\t"<<setw(75)<<"L O G G E D   I N   A S   S T U D E N T"<<endl;
    }
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<"\n\t"<<setw(60)<<"D A S H B O A R D"<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<"\n\t\tKEYS FOR THE OPERATIONS:"<<endl;
	cout<<"\t\t"<<string(40, '_')<<endl;
    if(allowed){
        cout<<"\n\t\tA: Add equipments to the data."<<endl;
    }
	cout<<"\t\tD: Display DashBoard."<<endl;
    if(allowed){
        cout<<"\t\tM: Modify the details of an equipment."<<endl;
    }
	cout<<"\t\tS: Search DashBoard."<<endl;
	cout<<"\t\tE: Logout."<<endl<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<"\n\t\tCHOOSE THE OPERATION THAT YOU WANT TO PERFORM : ";
	cin>>ch;
	cout<<endl;
	return ch;
}
char search_instructions(){
	char s;
	cout<<string(11, '\n');
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<"\n\t"<<setw(70)<<"S E A R C H   D A S H B O A R D"<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<"\n\t\t KEYS FOR SEARCH OPERATIONS:"<<endl;
	cout<<"\t\t"<<string(40, '_')<<endl;
	cout<<"\n\t\tI: Search by ID."<<endl;
	cout<<"\t\tD: Search by description."<<endl;
	cout<<"\t\tE: Move to the main operations chart."<<endl<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<"\n\t\tCHOOSE THE OPERATION THAT YOU WANT TO PERFORM : ";
	cin>>s;
	cout<<endl;
	return s;
}
char display_instructions(int allowed){
	char d;
	cout<<string(11, '\n');
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<"\n\t"<<setw(70)<<"D I S P L A Y   D A S H B O A R D"<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<"\n\t\t KEYS FOR DISPLAY OPERATIONS:"<<endl;
	cout<<"\t\t"<<string(40, '_')<<endl;
	cout<<"\n\t\tW: Whole Data"<<endl;
	cout<<"\t\tA: Available Equipments"<<endl;
	cout<<"\t\tN: Non-Available Equipments"<<endl;
    if(allowed){
        cout<<"\t\tD: Sorted Equipments by Date of Purchase"<<endl;
        cout<<"\t\tP: Equipments purchased over a Period"<<endl;
        cout<<"\t\tS: Amount Statistics"<<endl;
    }
	cout<<"\t\tE: Move to the main operations chart."<<endl<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<"\n\t\tCHOOSE THE OPERATION THAT YOU WANT TO PERFORM : ";
	cin>>d;
	cout<<endl;
	return d;
}

//************************//
//***** START SCREEN *****//
//************************//
void start_screen(){
	cout<<"\n\n\n\t"<<string(100, '_')<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<string(4, '\n');
	cout<<setw(90)<<string(65, '*')<<endl<<endl;
	cout<<setw(76)<<"D E A D S T O C K    R E G I S T E R"<<endl<<endl;
	cout<<setw(68)<<"M A C H I N E    L A B"<<endl<<endl;
	cout<<setw(90)<<string(65, '*')<<endl<<endl;
	cout<<setw(85)<<"DY Patil College Of Engineering, Akurdi"<<endl<<endl;
	cout<<setw(90)<<string(65, '*')<<endl<<endl;
	cout<<string(10, '\n');
	cout<<setw(90)<<"Project by DSCP-Group-4"<<endl;
	cout<<setw(90)<<string(30, '=')<<endl;
	cout<<setw(70)<<"Durva"<<setw(20)<<"72017999J"<<endl;
	cout<<setw(70)<<"Aishwarya"<<setw(20)<<"72018016D"<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
	cout<<"\t"<<string(100, '_')<<endl;
}

//*************************//
//***** MAIN FUNCTION *****//
//*************************//
int main() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	start_screen();
	Sleep(2500);
	system("cls");
    char l=' ';
    while(l!='E' && l!='e'){
        system("cls");
        l = login_instructions();
        switch(l){
            //******FOR ADMIN LOGIN******//
            case 'A':
            case 'a':
            {
                int cnt=0, v=0;
                while(cnt<4){
                    cnt+=1;
                    if(cnt<4){
                        cin.ignore();
                        v=login(cnt);
                        if(v==1){
                            break;
                        }
                    }else{
                    cout<<endl<<"\t"<<string(100, '_')<<endl;
                    SetConsoleTextAttribute(h, FOREGROUND_RED);
                    cout<<endl<<"\t\t\t**** OOPs!!! You failed logging in 3 times!!! Terminating program!!! ****"<<endl;
                    SetConsoleTextAttribute(h, 0x07);
                    cout<<"\n\n\n\n";
                    Sleep(2000);
                    }
                }
                if(v==1){
                    cout<<endl<<"\t"<<string(100, '_')<<endl;
                    cout<<"\n\t\t"<<"	Logging in as "<<user;
                    for(int j=0; j<6; j++){
                        cout<<".";  Sleep(500);
                    }
                    system("cls");
                    char ch=' ';
                    Equipment E;
                    All_equipments Equips;
                    const char *fileName;
                    fileName="ml_data.csv";
                    std::ifstream infile(fileName);
                    if(infile.good()){
                        read_data(Equips);
                    }
                    while(ch!='E' && ch!='e'){
                        ch=instructions(1,0);
                        if(ch=='E' or ch=='e'){
                            cout<<endl<<"\tLoging out";
                            for(int j=0; j<6; j++){
                                cout<<".";  Sleep(500);
                            }
                        }
                        switch(ch){
                            case 'A':
                            case 'a':
                                {			
                                    cin.ignore();
                                    system("cls");		
                                    add_n_equips(Equips);
                                    put_data(Equips);
                                    cout<<"\tAdding the data.";
                                    for(int j=0; j<6; j++){
                                        cout<<".";  Sleep(500);
                                    }
                                    system("cls");
                                    break;
                                }
                            case 'D':
                            case 'd':
                                {
                                    char d=' ';
                                    while(d!='E' && d!='e'){
                                        system("cls");
                                        d=display_instructions(1);
                                        switch(d){
                                            case 'p':
                                            case 'P':
                                            {
                                                string str;
                                                system("cls");
                                                display_period(Equips);
                                                cout<<"\tPress ENTER to go back to operations chart.";
                                                getline(cin, str);
                                                system("cls");
                                                break;
                                            }
                                            case 'W':
                                            case 'w':
                                                {
                                                    string str;
                                                    cout<<"\tDisplaying all the data";
                                                    for(int j=0; j<6; j++){
                                                        cout<<".";  Sleep(500);
                                                    }
                                                    Sleep(2000);
                                                    system("cls");
                                                    display_all(Equips, 1);
                                                    cout<<"\tPress ENTER to go back to operations chart.";
                                                    cin.ignore();
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;
                                                }
                                            case 'A':
                                            case 'a':
                                                {
                                                    string str;
                                                    cout<<"\tDisplaying data with status Available";
                                                    for(int j=0; j<6; j++){
                                                        cout<<".";  Sleep(500);
                                                    }
                                                    Sleep(2000);
                                                    system("cls");
                                                    display_avail(Equips, 1);
                                                    cout<<"\tPress ENTER to go back to operations chart.";
                                                    cin.ignore();
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;
                                                }
                                            case 'N':
                                            case 'n':
                                                {
                                                    string str;
                                                    cout<<"\tDisplaying data with status Not Available.";
                                                    for(int j=0; j<6; j++){
                                                        cout<<".";  Sleep(500);
                                                    }
                                                    Sleep(2000);
                                                    system("cls");
                                                    display_n_avail(Equips, 1);
                                                    cout<<"\tPress ENTER to go back to operations chart.";
                                                    cin.ignore();
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;
                                                }
                                            case 'S':
                                            case 's':
                                                {
                                                    string str;
                                                    cout<<"\tDisplaying amount statistics";
                                                    for(int j=0; j<6; j++){
                                                        cout<<".";  Sleep(500);
                                                    }
                                                    Sleep(2000);
                                                    system("cls");
                                                    display_statistics(Equips);
                                                    cout<<"\tPress ENTER to go back to operations chart.";
                                                    cin.ignore();
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;										
                                                } 
                                            case 'D':
                                            case 'd':
                                                {
                                                    string str;
                                                    All_equipments Sorted;
                                                    Sorted = sortData(Equips);
                                                    cout<<"\tDisplaying sorted data";
                                                    for(int j=0; j<6; j++){
                                                        cout<<".";  Sleep(500);
                                                    }
                                                    Sleep(2000);
                                                    system("cls");
                                                    display_sorted(Sorted);
                                                    cout<<"\tPress ENTER to go back to operations chart.";
                                                    cin.ignore();
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;										
                                                }    	
                                            }
                                    }
                                    break;
                                }
                            case 'M':
                            case 'm':
                                {
                                    system("cls");
                                    cin.ignore();
                                    modify(Equips);
                                    put_data(Equips);
                                    system("cls");
                                    break;
                                }
                            case 'S':
                            case 's':
                                {
                                    char s=' ';
                                    while(s!='E' && s!='e'){
                                        system("cls");
                                        s=search_instructions();
                                        switch(s){
                                            case 'I':
                                            case 'i':
                                                {
                                                    string str;
                                                    search_by_id(Equips,1);
                                                    cout<<endl<<"\tPress ENTER to close the display.";
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;
                                                }
                                            case 'D':
                                            case 'd':
                                                {
                                                    string str;
                                                    search_by_description(Equips, 1);
                                                    cout<<endl<<"\tPress ENTER to close the display.";
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;
                                                }		
                                        }	
                                    }
                                    break;
                                }
                        }
                    }
                }else{
                    break;
                }
                break;
            }
 
            //*******FOR LAB ASSISTANT LOGIN******//
            case 'L':
            case 'l':
            {
                int cnt=0, v=0;
                while(cnt<4){
                    cnt+=1;
                    cin.ignore();
                    if(cnt<4){
                        v=login_assistant(cnt);
                        if(v==1){
                            break;
                        }
                    }else{
                    cout<<endl<<"\t"<<string(100, '_')<<endl;
                    SetConsoleTextAttribute(h, FOREGROUND_RED);
                    cout<<endl<<"\t\t\t**** OOPs!!! You failed logging in 3 times!!! Terminating program!!! ****"<<endl;
                    SetConsoleTextAttribute(h, 0x07);
                    cout<<"\n\n\n\n";
                    cin.ignore();
                    }
                }
                if(v==1){
                    cout<<endl<<"\t"<<string(100, '_')<<endl;
                    cout<<"\n\t\t"<<"	Logging in as "<<assistant;
                    for(int j=0; j<6; j++){
                        cout<<".";  Sleep(500);
                    }
                    system("cls");
                    char ch=' ';
                    Equipment E;
                    All_equipments Equips;
                    const char *fileName;
                    fileName="ml_data.csv";
                    std::ifstream infile(fileName);
                    if(infile.good()){
                        read_data(Equips);
                    }
                    while(ch!='E' && ch!='e'){
                        ch=instructions(0, 0);
                        if(ch=='E' or ch=='e'){
                            cout<<endl<<"\tLoging out";
                            for(int j=0; j<6; j++){
                                cout<<".";  Sleep(500);
                            }
                        }
                        switch(ch){
                        case 'D':
                        case 'd':
                                {
                                    char d=' ';
                                    while(d!='E' && d!='e'){
                                        system("cls");
                                        d=display_instructions(1);
                                        switch(d){
                                            case 'p':
                                            case 'P':
                                            {
                                                string str;
                                                system("cls");
                                                display_period(Equips);
                                                cout<<"\tPress ENTER to go back to operations chart.";
                                                getline(cin, str);
                                                system("cls");
                                                break;
                                            }
                                            case 'W':
                                            case 'w':
                                                {
                                                    string str;
                                                    cout<<"\tDisplaying all the data";
                                                    for(int j=0; j<6; j++){
                                                        cout<<".";  Sleep(500);
                                                    }
                                                    Sleep(2000);
                                                    system("cls");
                                                    display_all(Equips, 1);
                                                    cout<<"\tPress ENTER to go back to operations chart.";
                                                    cin.ignore();
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;
                                                }
                                            case 'A':
                                            case 'a':
                                                {
                                                    string str;
                                                    cout<<"\tDisplaying data with status Available";
                                                    for(int j=0; j<6; j++){
                                                        cout<<".";  Sleep(500);
                                                    }
                                                    Sleep(2000);
                                                    system("cls");
                                                    display_avail(Equips, 1);
                                                    cout<<"\tPress ENTER to go back to operations chart.";
                                                    cin.ignore();
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;
                                                }
                                            case 'N':
                                            case 'n':
                                                {
                                                    string str;
                                                    cout<<"\tDisplaying data with status Not Available.";
                                                    for(int j=0; j<6; j++){
                                                        cout<<".";  Sleep(500);
                                                    }
                                                    Sleep(2000);
                                                    system("cls");
                                                    display_n_avail(Equips, 1);
                                                    cout<<"\tPress ENTER to go back to operations chart.";
                                                    cin.ignore();
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;
                                                }
                                            case 'S':
                                            case 's':
                                                {
                                                    string str;
                                                    cout<<"\tDisplaying amount statistics";
                                                    for(int j=0; j<6; j++){
                                                        cout<<".";  Sleep(500);
                                                    }
                                                    Sleep(2000);
                                                    system("cls");
                                                    display_statistics(Equips);
                                                    cout<<"\tPress ENTER to go back to operations chart.";
                                                    cin.ignore();
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;										
                                                } 
                                            case 'D':
                                            case 'd':
                                                {
                                                    string str;
                                                    All_equipments Sorted;
                                                    Sorted = sortData(Equips);
                                                    cout<<"\tDisplaying sorted data";
                                                    for(int j=0; j<6; j++){
                                                        cout<<".";  Sleep(500);
                                                    }
                                                    Sleep(2000);
                                                    system("cls");
                                                    display_sorted(Sorted);
                                                    cout<<"\tPress ENTER to go back to operations chart.";
                                                    cin.ignore();
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;										
                                                }    	
                                            }
                                    }
                                    break;
                                }
                    
                        case 'S':
                        case 's':
                                {
                                    char s=' ';
                                    while(s!='E' && s!='e'){
                                        system("cls");
                                        s=search_instructions();
                                        switch(s){
                                            case 'I':
                                            case 'i':
                                                {
                                                    string str;
                                                    search_by_id(Equips,1);
                                                    cout<<endl<<"\tPress ENTER to close the display.";
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;
                                                }
                                            case 'D':
                                            case 'd':
                                                {
                                                    string str;
                                                    search_by_description(Equips,1);
                                                    cout<<endl<<"\tPress ENTER to close the display.";
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;
                                                }		
                                        }	
                                    }
                                    break;
                                }
                    
                        }
                    }
                }else{
                    break;
                }
                break;
            }
            
            //*******FOR STUDENT LOGIN*******//
            case 'S':
            case 's':
            {
                cout<<"\n\t\t"<<"	Logging in as Student";
                for(int j=0; j<6; j++){
                    cout<<".";  Sleep(500);
                }
                system("cls");
                char ch=' ';
                Equipment E;
                All_equipments Equips;
                const char *fileName;
                fileName="ml_data.csv";
                std::ifstream infile(fileName);
                if(infile.good()){
                    read_data(Equips);
                }
                while(ch!='E' && ch!='e'){
                        ch=instructions(0, 1);
                        if(ch=='E' or ch=='e'){
                            cout<<endl<<"\tLoging out";
                            for(int j=0; j<6; j++){
                                cout<<".";  Sleep(500);
                            }
                        }
                        switch(ch){
                        case 'D':
                        case 'd':
                                {
                                    char d=' ';
                                    while(d!='E' && d!='e'){
                                        system("cls");
                                        d=display_instructions(0);
                                        switch(d){
                                            case 'W':
                                            case 'w':
                                                {
                                                    string str;
                                                    cout<<"\tDisplaying all the data";
                                                    for(int j=0; j<6; j++){
                                                        cout<<".";  Sleep(500);
                                                    }
                                                    Sleep(2000);
                                                    system("cls");
                                                    display_all(Equips, 0);
                                                    cout<<"\tPress ENTER to go back to operations chart.";
                                                    cin.ignore();
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;
                                                }
                                            case 'A':
                                            case 'a':
                                                {
                                                    string str;
                                                    cout<<"\tDisplaying data with status Available";
                                                    for(int j=0; j<6; j++){
                                                        cout<<".";  Sleep(500);
                                                    }
                                                    Sleep(2000);
                                                    system("cls");
                                                    display_avail(Equips, 0);
                                                    cout<<"\tPress ENTER to go back to operations chart.";
                                                    cin.ignore();
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;
                                                }
                                            case 'N':
                                            case 'n':
                                                {
                                                    string str;
                                                    cout<<"\tDisplaying data with status Not Available.";
                                                    for(int j=0; j<6; j++){
                                                        cout<<".";  Sleep(500);
                                                    }
                                                    Sleep(2000);
                                                    system("cls");
                                                    display_n_avail(Equips, 0);
                                                    cout<<"\tPress ENTER to go back to operations chart.";
                                                    cin.ignore();
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;
                                                }
                                            }
                                    }
                                    break;
                                }
                    
                        case 'S':
                        case 's':
                                {
                                    char s=' ';
                                    while(s!='E' && s!='e'){
                                        system("cls");
                                        s=search_instructions();
                                        switch(s){
                                            case 'I':
                                            case 'i':
                                                {
                                                    string str;
                                                    search_by_id(Equips,0);
                                                    cout<<endl<<"\tPress ENTER to close the display.";
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;
                                                }
                                            case 'D':
                                            case 'd':
                                                {
                                                    string str;
                                                    search_by_description(Equips,0);
                                                    cout<<endl<<"\tPress ENTER to close the display.";
                                                    getline(cin, str);
                                                    system("cls");
                                                    break;
                                                }		
                                        }	
                                    }
                                    break;
                                }
                    
                        }
                    }
                    break;
            }
       
        }//End of major switch case
    }//End of major while loop
    
    return 0;
}

