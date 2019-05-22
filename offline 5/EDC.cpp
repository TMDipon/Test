#include<bits/stdc++.h>
#include<string>
#include<vector>
#include<algorithm>
#include<stdlib.h>

using namespace std;

struct group_char
{
	string str;
	vector<int> bits;
};

vector<struct group_char> a;
vector<struct group_char> b;
struct group_char cwcheck;	//this will hold the serialized data
struct group_char cwcheck1;	//this will be used to show the bits which didn't match with the original data(in red color) 
int len;
string padd_msg(string s,int m)
{
	if((s.size()%m) == 0)
	{
		return s;
	}
	else
	{
		int f = m - (s.size()%m);
		for(int i=0;i<f;i++)
		{
			s = s + '~';
		}
		return s;
	}
} 

void Convert_to_Byte(string s,int m)
{
	for(int i=0;i<s.size();i=i+m)
	{
		struct group_char b;
		b.str = "";
		for(int j=0;j<m;j++)
		{
			b.str = b.str + s[i+j];
			int val = (int)(s[i+j]);
			int k=0;
			while(val > 0)
			{
				if((val % 2) == 0){b.bits.push_back(0);}
				else if((val%2) == 1){b.bits.push_back(1);}
				val /= 2;
				k++;
			}

			for(int l=0;l< (8-k);l++)
			{
				b.bits.push_back(0);
			}
			reverse(b.bits.begin() + (j*8),b.bits.begin() + (j*8)+8);
		}
		a.push_back(b);
	}
}

void add_checksum()
{
	for(int i=0;i<a.size();i++)
	{
		int l=1;
		for(int j=0;j<a[i].bits.size();j++)
		{
			if(((j+1) % l) == 0)
			{
				a[i].bits.insert(a[i].bits.begin()+j,-1);
				l=l*2;
			}
		}

		int l1 = 1;
		while(l1 < l)
		{
			int count = 0;
			for(int x = l1;x<=a[i].bits.size();x += 2*l1)
			{
				int y = x;
				while(y < x+l1 && y <= a[i].bits.size())
				{
					if(a[i].bits[y-1] == 1){count++;}
					y++;
				}
			}		

			a[i].bits[l1-1] = (count % 2);
			l1 = l1*2;
		}
	}
}

void Serialize()			//Column wise serialization
{
	int n = a[0].bits.size();
	for(int i =0;i<n;i++)
	{
		for(int j=0;j<a.size();j++)
		{
			cwcheck.bits.push_back(a[j].bits[i]);
			cwcheck1.bits.push_back(a[j].bits[i]);
		}
	}
}


void CRC_Calculate(string pol)
{
		char key[pol.length()+1];
		strcpy(key,pol.c_str());

		char key1[pol.length()+1];
		strcpy(key1,key);
		char temp[20],quot[2000];
		int keylen = pol.length();
		char rem[keylen+1];
		int msglen = cwcheck.bits.size();

		for (int i=0;i<keylen-1;i++)
	        {
			cwcheck.bits.push_back(0);
		}

		for (int i=0;i<keylen;i++)
		{
			temp[i] = (char)(cwcheck.bits[i]+48);
		}
		for (int i=0;i<msglen;i++)
	        {
			quot[i]=temp[0];

			if(quot[i] == '0')
			{
				for (int j=0;j<keylen;j++)
				{
					key[j]='0';
				}
			}
		        else
			{
				for (int j=0;j<keylen;j++)
				{
			 		key[j]=key1[j];
				}
			}
			for (int j=keylen-1;j>0;j--) 
			{
				if(temp[j] == key[j])
				 rem[j-1]='0'; else
				 rem[j-1]='1';
			}
			rem[keylen-1]=(char)(cwcheck.bits[i+keylen]+48);
			strcpy(temp,rem);
		}

		//rem = temp;
		int i;
		for(i = 0; temp[i] != '\0'; i++)
			rem[i] = temp[i];
		rem[i-1] = '\0';
		len = i-1;
		int x = cwcheck.bits.size() - msglen;
		for(int i=0;i<x;i++)
		{
			cwcheck.bits.pop_back();
		}

		for(int j=0;rem[j] != '\0';j++)
		{
			cwcheck.bits.push_back(rem[j]-48);
			cwcheck1.bits.push_back(rem[j]-48);
		}
		cout<<endl;
}

int Error_Detect(string pol)			//Same as CRC Calculation just checking whether the received frame is divisible by the polynomial
{
		char key[pol.length()+1];
		strcpy(key,pol.c_str());

		char key1[pol.length()+1];
		strcpy(key1,key);
		char temp[20],quot[2000];
		int keylen = pol.length();
		char rem[keylen+1];
		int msglen = cwcheck.bits.size();

		for (int i=0;i<keylen-1;i++)
	        {
			cwcheck.bits.push_back(0);
		}

		for (int i=0;i<keylen;i++)
		{
			temp[i] = (char)(cwcheck.bits[i]+48);
		}
		for (int i=0;i<msglen;i++)
	        {
			quot[i]=temp[0];

			if(quot[i] == '0')
			{
				for (int j=0;j<keylen;j++)
				{
					key[j]='0';
				}
			}
		        else
			{
				for (int j=0;j<keylen;j++)
				{
			 		key[j]=key1[j];
				}
			}
			for (int j=keylen-1;j>0;j--) 
			{
				if(temp[j] == key[j])
				 rem[j-1]='0'; else
				 rem[j-1]='1';
			}
			rem[keylen-1]=(char)(cwcheck.bits[i+keylen]+48);
			strcpy(temp,rem);
		}

		//rem = temp;
		int i;
		for(i = 0; temp[i] != '\0'; i++)
			rem[i] = temp[i];
		rem[i-1] = '\0';
		len = i-1;
		int x = cwcheck.bits.size() - msglen;
		for(int i=0;i<x;i++)
		{
			cwcheck.bits.pop_back();
		}

		int z=0;
		for(int j=0;rem[j] != '\0';j++)
		{
			if(rem[j] == '1')
			{
				z = 1;
				break;
			}
		}
		cout<<endl;
		return z;
}

void Transmit(double p)
{
	if(p != 0.0)
	{
		int pr = (int)(p*100);
		srand (time(NULL));
	
		for(int i=0;i<cwcheck.bits.size();i+=100)
		{
			if((i+99) < cwcheck.bits.size())
			{
				int x = (100/pr);
				for(int j=i;j<i+100;j+=x)
				{
					int idx = (rand() % (j+x)) + j;
					if(cwcheck.bits[idx] == 0)
					{
						cwcheck.bits[idx] = 1;
					}
					else
					{
						cwcheck.bits[idx] = 0;
					}
				}
			}
			else
			{
				int x = ((cwcheck.bits.size()-i)/pr);
				for(int j=i;j<cwcheck.bits.size();j+=x)
				{
					int idx = (rand() % (j+x)) + j;
					if(cwcheck.bits[idx] == 0)
					{
						cwcheck.bits[idx] = 1;
					}
					else
					{
						cwcheck.bits[idx] = 0;
					}
				}
			}
		}
	}
}

void De_Serialize()
{
	int x = a.size();
	for(int i=0;i<len;i++)
	{
		cwcheck.bits.pop_back();
	}

	for(int i=0;i<x;i++)
	{
		struct group_char temp;
		b.push_back(temp);
	}
	
	for(int i=0,j=0;i<cwcheck.bits.size();i++)
	{
		b[j].bits.push_back(cwcheck.bits[i]);
		j++;
		if(j%x == 0){j=0;}
	}	
}

void Error_Correct()
{
	for(int i=0;i<b.size();i++)
	{
		int l=1;
		for(int j=0;j<b[i].bits.size();j++)
		{
			if(((j+1) % l) == 0)
			{
				l=l*2;
			}
		}

		int l1 = 1;
		int count = 0;
		while(l1 < l)
		{
			int c = 0;
			for(int x = l1;x<=b[i].bits.size();x += 2*l1)
			{
				int y = x;
				while(y < x+l1 && y <= b[i].bits.size())
				{
					if(b[i].bits[y-1] == 1){c++;}
					y++;
				}
			}		

			if(c%2 == 1)
			{
				count = count + l1;
			}
			l1 = l1*2;
		}

		if(count !=0)
		{
			if(b[i].bits[count - 1] == 0)
			{
				b[i].bits[count-1] = 1;
			}
			else
			{
				b[i].bits[count-1] = 0;
			}
		}

		l=1;
		for(int j=0;j<b[i].bits.size();j++)
		{
			if(((j+1) % l) == 0)
			{
				b[i].bits[j] = 2;
				l=l*2;
			}
		}

		int l2=0;
		while(l2 < b[i].bits.size())
		{
			if(b[i].bits[l2] == 2)
			{
				b[i].bits.erase(b[i].bits.begin() + l2);
			}
			else
			{
				l2++;
			}
		}		
	}
}

string Extract_Result()
{
	string res = "";
	int l,val;
	for(int i=0;i<b.size();i++)
	{
		for(int j=0;j<b[i].bits.size();j+=8)
		{
			l = 128;
			val = 0;
			for(int k=j;k<j+8;k++)
			{
				val = val + (l * b[i].bits[k]);
				l = l/2;
			}
			char c = val;
			res += c;
		}
	}
	return res;
}
		
void print(int x)
{

	if(x == 0)
	{
		for(int i=0;i<a.size();i++)
		{
			for(int j=0;j<a[i].bits.size();j++)
			{
				cout<<a[i].bits[j];
			}
			cout<<endl;
		}
		cout<<"\n";
	}
	else if(x == 1)
	{
		for(int i=0;i<a.size();i++)
		{
			int l=1;
			for(int j=0;j<a[i].bits.size();j++)
			{
				if((j+1)%l == 0)
				{
					//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					string e = "\033[0;32m"+to_string(a[i].bits[j]);
					cout<<e;
					l=l*2;
				}
				else
				{
					string e = "\033[0m"+to_string(a[i].bits[j]);
					cout<<e;
				}
			}
			cout<<endl;
		}
		cout<<"\n";
	}
	else if(x==2)
	{
		for(int j=0;j<cwcheck.bits.size();j++)
		{
			cout<<cwcheck.bits[j];
		}	
		cout<<"\n";
	}
	else if(x==3)
	{
		for(int j=0;j<cwcheck.bits.size()-len;j++)
		{
			cout<<cwcheck.bits[j];
		}

		for(int j=cwcheck.bits.size()-len;j<cwcheck.bits.size();j++)
		{
			string e = "\033[0;36m"+to_string(cwcheck.bits[j]);
			cout<<e;
		}
		cout<<"\n";	
	}
	else if(x==4)
	{
		for(int j=0;j<cwcheck.bits.size();j++)
		{
			if(cwcheck.bits[j] != cwcheck1.bits[j])
			{
				string e = "\033[0;31m"+to_string(cwcheck.bits[j]);
				cout<<e;
			}
			else
			{
				string e = "\033[0m"+to_string(cwcheck.bits[j]);
				cout<<e;
			}	
		}	
		cout<<"\n";
	}
	else if(x==5)
	{
		for(int i=0;i<b.size();i++)
		{
			for(int j=0;j<b[i].bits.size();j++)
			{
				if(b[i].bits[j] != a[i].bits[j])
				{
					string e = "\033[0;31m"+to_string(b[i].bits[j]);
					cout<<e;
				}
				else
				{
					string e = "\033[0m"+to_string(b[i].bits[j]);
					cout<<e;
				}
			}
			cout<<endl;
		}
		cout<<"\n";
	}
	else if(x==6)
	{
		for(int i=0;i<b.size();i++)
		{
			for(int j=0;j<b[i].bits.size();j++)
			{
				cout<<b[i].bits[j];
			}
			cout<<endl;
		}
		cout<<"\n";
	}
}			


int main()
{
	string s,pol;
	int m;
	double prob;
	cout<<"Enter the data string : ";
	getline(cin,s);
	cout<<"\nEnter number of data bytes in a row<m> : ";
	cin>>m;
	cout<<"\nEnter probability<p> : ";
	cin>>prob;
	cout<<"\nEnter generator polynomial : ";
	cin>>pol;
	string g ="\033[0m";	

	string modified = padd_msg(s,m);
	cout<<"\nData string after padding : "<<modified<<"\n\n";
	
	//------------Task1 ends--------------

	Convert_to_Byte(modified,m);
	cout<<"\nData block <ascii code of m characters per row> :\n";
	print(0);

	//------------Task2 ends--------------
	add_checksum();
	cout<<"\nData block after adding check bits :\n";
	print(1);

	//------------Task3 ends--------------
	Serialize();
	cout<<g<<"\nData bits after column wise serialization :\n";
	print(2);
	
	//------------Task4 Ends--------------
	CRC_Calculate(pol);
	cout<<g<<"\nData bits after appending CRC checksum <sent frame> :\n";
	print(3);

	//------------Task5 Ends--------------
	Transmit(prob);
	cout<<g<<"\nReceived Frame :\n";
	print(4);

	//------------Task6 Ends--------------
	int p = Error_Detect(pol);
	if(p == 1)
	{
		cout<<g<<"\nResult of CRC checksum matching: Error Detected\n";
	}	
	else
	{
		cout<<g<<"\nResult of CRC checksum matching: No Error Detected\n";
	}

	//------------Task7 Ends---------------
	De_Serialize();
	cout<<g<<"\nData block after removing CRC checksum bits :\n";
	print(5);

	//------------Task8 Ends---------------
	Error_Correct();
	cout<<g<<"\nData block after removing check bits :\n";
	print(6);

	//------------Task9 Ends---------------
	string res_msg = Extract_Result();
	cout<<g<<"\n\nOutput frame : "<<res_msg;

	return 0;
}
