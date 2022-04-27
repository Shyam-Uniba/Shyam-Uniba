// use central position calculate pad coordinate
// 2021.03 weixl
//   ^       _________
//  Z|  [3] |         | [2]
//   |      |         |
//   |      |         |
//   |      |         |
//   |  [0] |_________| [1]    (start point [0] is in the left bottum)
//   |
//   |-----------------------------> Y
//

#include <iostream>
#include <fstream>
#include <string>

//using namespace std;
void convert_new()
{
	
	ofstream outfile1("PadCoordinate.txt");
	//ofstream outfile2("padleft.txt");
	outfile1<<"PadID"<<"\t"<<"Sector"<<"\t"<<"Row"<< setw(10) <<"PadInRow"<<setw(10) << "gCenX" << setw(12) << "gCenZ" <<setw(12)<<"gx[0]"<<setw(12)<<"gz[0]"<<setw(12)<<"gx[1]"<<setw(12)<<"gz[1]"<<setw(12)<<"gx[2]"<<setw(12)<<"gz[2]"<<setw(12)<<"gx[3]"<<setw(12)<<"gz[3]"<<setw(12)<<"lCenX" << setw(12) <<"lCenZ" << setw(12) << "lx[0]" << setw(12) << "lz[0]" <<setw(12)<<"lx[1]" << setw(12) << "lz[1]" << setw(12) << "lx[2]" << setw(12) << "lz[2]" << setw(12) << "lx[3]" << setw(12) << "lz[3]" <<endl;
	//"\n"<<"sector-row-pad"<<setw(10)<<"x[4]"<<setw(10)<<"z[4]"<<setw(10)<<"x[5]"<< setw(10) <<"z[5]"<<setw(10)<<"x[6]"<<setw(10)<<"z[6]"<<setw(10)<<"x[7]"<<setw(10)<<"z[7]"<<
	//outfile2<<"PadID"<<setw(10)<<"y[0]"<<setw(10)<<"z[0]"<<setw(10)<<"y[1]"<<setw(10)<<"z[1]"<<setw(10)<<"y[2]"<<setw(10)<<"z[2]"<<setw(10)<<"y[3]"<<setw(10)<<"z[3]"<<endl;
	
	double length=900.0,width=500.0;
	
	
	double dl=14.3, ds=5.0;  //forward angle parameters
	double ul=12.5, us=4.0;  //large angle parameters
	
	
	int dRowSum=int(width/dl);
	const int drs=dRowSum;
	int PointNum1[drs];
	int PointNum2[drs];
	int ds1=0;

	for(int dh=1;dh<drs+1;dh++)
	{
		PointNum1[dh]=int((200+(dl/2)+dl*(dh-1))/ds);
		ds1=ds1+PointNum1[dh];	
	}
	
	const int dtp=ds1;
    double dyc[dtp],dzc[dtp];
	double dy0[dtp],dz0[dtp];
    double dy1[dtp],dz1[dtp];
    double dy2[dtp],dz2[dtp];
    double dy3[dtp],dz3[dtp];
	int id1 = 0;
	for(int i=1;i<dtp+1;i++)
	{
		int ds2=0;
		int u,v;
		if (i == dtp) {
			id1 = i;//for pad number
		}

		for(int dr=1;dr<drs+1;dr++)
	    {
		PointNum2[dr]=int((200+(dl/2)+dl*(dr-1))/ds);
		int n1 = i - ds2-1;//for pad in row
		ds2=ds2+PointNum2[dr];
        if(i-ds2<=0)
			{
				u=dr;
				v=i-(ds2-PointNum2[dr]);
				dyc[i]=100+(dl/2)+dl*(u-1);
		        dzc[i]=dyc[i]-(ds/2)-ds*(v-1);
				cout<<i<<"\t"<<"dyc"<<"["<<i<<"]"<<"\t"<<dyc[i]<<"\t"<<"dzc"<<"["<<i<<"]"<<"\t"<<dzc[i]<<endl;
				
				dy0[i] = (dyc[i]-(dl/2))/10; 
				dy1[i] = (dyc[i]+(dl/2))/10;
				dy2[i] = (dyc[i]+(dl/2))/10;
				dy3[i] = (dyc[i]-(dl/2))/10; 
		
				dz0[i] = (dzc[i]+(ds/2)-((dl/2)*((dzc[i]+(ds/2))/dyc[i])) - 350)/10;
				dz1[i] = (dzc[i]+(ds/2)+((dl/2)*((dzc[i]+(ds/2))/dyc[i])) - 350)/10;
				dz2[i] = (dzc[i]-(ds/2)+((dl/2)*((dzc[i]-(ds/2))/dyc[i])) - 350)/10;
				dz3[i] = (dzc[i]-(ds/2)-((dl/2)*((dzc[i]-(ds/2))/dyc[i])) - 350)/10;
				double Center1X = (dy0[i] + dy1[i] + dy2[i] + dy3[i]) / 4;
				double Center1Z = (dz0[i] + dz1[i] + dz2[i] + dz3[i]) / 4;
					outfile1 << i-1 << "\t" << "0"<<"\t" << dr-1 << setw(10) << n1 << setw(12) << Center1X << setw(12) << Center1Z  << setw(12) << dy0[i] << setw(12) << dz0[i] << setw(12) << dy1[i] << setw(12) << dz1[i] << setw(12) << dy2[i] << setw(12) << dz2[i] << setw(12) << dy3[i] << setw(12) << dz3[i] << setw(12) << -Center1Z << setw(12) << Center1X << setw(12) << -dz0[i] << setw(12) << dy0[i] << setw(12) << -dz1[i] << setw(12) << dy1[i] << setw(12) << -dz2[i] << setw(12) << dy2[i] << setw(12) << -dz3[i] << setw(12) << dy3[i] <<endl;
					//i << "\t" << "sector-row" << setw(12) << -dy0[i] << setw(12) << dz0[i] << setw(12) << -dy1[i] << setw(12) << dz1[i] << setw(12) << -dy2[i] << setw(12) << dz2[i] << setw(12) << -dy3[i] << setw(12) << dz3[i] <<
					//outfile2<<i<<setw(12)<<-dy0[i]<<setw(12)<<dz0[i]<<setw(12)<<-dy1[i]<<setw(12)<<dz1[i]<<setw(12)<<-dy2[i]<<setw(12)<<dz2[i]<<setw(12)<<-dy3[i]<<setw(12)<<dz3[i]<<endl;
				
				break;
			}
				
		else
			continue;
	    }
	}
	
	int uRowSum=int((length-200.0)/ul);
	const int urs=uRowSum;
	int uPointNum1[urs];
	int uPointNum2[urs];
	int us1=0;
	
    for(int uh=1;uh<urs+1;uh++)
	{
		if(((ul/2)+ul*(uh-1)-width)<0)
		{
			uPointNum1[uh]=int(((ul/2)+ul*(uh-1))/us);
		}
		else if(((ul/2)+ul*(uh-1)-width)>=0)
		{
			uPointNum1[uh]=int(width/us);
		}	
		us1=us1+uPointNum1[uh];	
	}
	
	const int utp=us1;
	double uyc[utp],uzc[utp];
	double uy0[utp],uz0[utp];
    double uy1[utp],uz1[utp];
    double uy2[utp],uz2[utp];
    double uy3[utp],uz3[utp];
	int id3 = 0;
	for(int j=1;j<utp+1;j++)
	{
		int us2=0;
		int u,v;
		if (j == utp) {
			id3 = j + id1;
		}
		for(int ur=1;ur<urs+1;ur++)
	    {
			
			if(((ul/2)+ul*(ur-1)-width)<0)
			{
				uPointNum2[ur]=int(((ul/2)+ul*(ur-1))/us);
			}
			else if(((ul/2)+ul*(ur-1)-width)>=0)
			{
				uPointNum2[ur]=int(width/us);
			}
			int n2 = j - us2-1; //for pad in row
			us2=us2+uPointNum2[ur];//us2-1为当前sector到当前row的总pad数;uPointNum为当前row的pad数
			if(j-us2<=0)
			{
				u=ur;   
				//v=j-(us2-uPointNum2[ur]);
				v = us2 - j + 1;
				uzc[j]=100+(ul/2)+ul*(u-1);
				if(((ul/2)+ul*(ur-1)-width)<0)
				{
					uyc[j]=uzc[j]-(us/2)-us*(v-1);
				}
		        else if(((ul/2)+ul*(ur-1)-width)>=0)
				{
					uyc[j]=width-(us/2)-us*(v-1)+100;
				}
			
				 cout<<j<<"\t"<<"uyc"<<"["<<j<<"]"<<"\t"<<uyc[j]<<"\t"<<"uzc"<<"["<<j<<"]"<<"\t"<<uzc[j]<<endl;

				//cout<<j<<"uyc[j]"<<uyc[j]<<"uzc[j]"<<uzc[j]<<endl;
				
				uy0[j] = (uyc[j]-(us/2)-((ul/2)*((uyc[j]-(us/2))/uzc[j]))) / 10;
				uy1[j] = (uyc[j]-(us/2)+((ul/2)*((uyc[j]-(us/2))/uzc[j])))/10;
				uy2[j] = (uyc[j]+(us/2)+((ul/2)*((uyc[j]+(us/2))/uzc[j])))/10;
				uy3[j] = (uyc[j]+(us/2)-((ul/2)*((uyc[j]+(us/2))/uzc[j]))) / 10;
				
				uz0[j] = (uzc[j]-(ul/2) - 350) / 10;
				uz1[j] = (uzc[j]+(ul/2) - 350) / 10;
				uz2[j] = (uzc[j]+(ul/2) - 350) / 10;
				uz3[j] = (uzc[j]-(ul/2) - 350) / 10;
				int id2 = j + id1 -1;
				double Center2X = (uy0[j] + uy1[j] + uy2[j] + uy3[j]) / 4;
				double Center2Z = (uz0[j] + uz1[j] + uz2[j] + uz3[j]) / 4;
				outfile1<<id2<<"\t" << "1" <<"\t"<< ur-1 << setw(10) << n2 << setw(12) << Center2X << setw(12) << Center2Z <<setw(12)<<uy0[j]<<setw(12)<<uz0[j]<<setw(12)<<uy1[j]<<setw(12)<<uz1[j]<<setw(12)<<uy2[j]<<setw(12)<<uz2[j]<<setw(12)<<uy3[j]<<setw(12)<<uz3[j] << setw(12) << Center2X << setw(12) << Center2Z << setw(12) << uy0[j] << setw(12) << uz0[j] << setw(12) << uy1[j] << setw(12) << uz1[j] << setw(12) << uy2[j] << setw(12) << uz2[j] << setw(12) << uy3[j] << setw(12) << uz3[j] <<endl;
				//ID<<setw(10)<<"sector-row"<<setw(12)<<-uy0[j]<<setw(12)<<uz0[j]<<setw(12)<<-uy1[j]<<setw(12)<<uz1[j]<<setw(12)<<-uy2[j]<<setw(12)<<uz2[j]<<setw(12)<<-uy3[j]<<setw(12)<<uz3[j]<<
				//outfile2<<j+2997<<setw(12)<<-uy0[j]<<setw(12)<<uz0[j]<<setw(12)<<-uy1[j]<<setw(12)<<uz1[j]<<setw(12)<<-uy2[j]<<setw(12)<<uz2[j]<<setw(12)<<-uy3[j]<<setw(12)<<uz3[j]<<endl;
				
				break;
			}
		else
			continue;
	    }
	}

	int id5 = 0;
	for (int k = 1; k < utp + 1; k++)
	{
		int us2 = 0;
		int u, v;
		if (k == utp) {
			id5 = k + id3;
		}
		for (int ur = 1; ur < urs + 1; ur++)
		{

			if (((ul / 2) + ul * (ur - 1) - width) < 0)
			{
				uPointNum2[ur] = int(((ul / 2) + ul * (ur - 1)) / us);
			}
			else if (((ul / 2) + ul * (ur - 1) - width) >= 0)
			{
				uPointNum2[ur] = int(width / us);
			}
			int n3 = k - us2-1;//for pad in row
			us2 = us2 + uPointNum2[ur];
			if (k - us2 <= 0)
			{
				u = ur;
				v = k - (us2 - uPointNum2[ur]);
				uzc[k] = 100 + (ul / 2) + ul * (u - 1);
				if (((ul / 2) + ul * (ur - 1) - width) < 0)
				{
					uyc[k] = uzc[k] - (us / 2) - us * (v - 1);
				}
				else if (((ul / 2) + ul * (ur - 1) - width) >= 0)
				{
					uyc[k] = width - (us / 2) - us * (v - 1) + 100;
				}

				cout<<k<<"\t"<<"uyc"<<"["<<k<<"]"<<"\t"<<uyc[k]<<"\t"<<"uzc"<<"["<<k<<"]"<<"\t"<<uzc[k]<<endl;

				//cout<<k<<"uyc[k]"<<uyc[k]<<"uzc[k]"<<uzc[k]<<endl;

				uy0[k] =( uyc[k] + (us / 2) - ((ul / 2) * ((uyc[k] + (us / 2)) / uzc[k]))) / 10;
				uy1[k] =( uyc[k] + (us / 2) + ((ul / 2) * ((uyc[k] + (us / 2)) / uzc[k]))) / 10;
				uy2[k] =( uyc[k] - (us / 2) + ((ul / 2) * ((uyc[k] - (us / 2)) / uzc[k]))) / 10;
				uy3[k] =( uyc[k] - (us / 2) - ((ul / 2) * ((uyc[k] - (us / 2)) / uzc[k]))) / 10;

				uz0[k] =( uzc[k] - (ul / 2) - 350) / 10;
				uz1[k] =( uzc[k] + (ul / 2) - 350) / 10;
				uz2[k] =( uzc[k] + (ul / 2) - 350) / 10;
				uz3[k] =( uzc[k] - (ul / 2) - 350) / 10;

				int id4 = k + id3-1;
				double Center3X = (uy0[k] + uy1[k] + uy2[k] + uy3[k]) / 4;
				double Center3Z = (uz0[k] + uz1[k] + uz2[k] + uz3[k]) / 4;
				outfile1<<id4<<"\t" << "2" <<"\t"<< ur-1<<setw(10) << n3 << setw(12) << -Center3X << setw(12) << Center3Z <<setw(12)<<-uy0[k]<<setw(12)<<uz0[k]<<setw(12)<<-uy1[k]<<setw(12)<<uz1[k]<<setw(12)<<-uy2[k]<<setw(12)<<uz2[k]<<setw(12)<<-uy3[k]<<setw(12)<<uz3[k] << setw(12) << -Center3X << setw(12) << Center3Z << setw(12) << -uy0[k] << setw(12) << uz0[k] << setw(12) << -uy1[k] << setw(12) << uz1[k] << setw(12) << -uy2[k] << setw(12) << uz2[k] << setw(12) << -uy3[k] << setw(12) << uz3[k] <<endl;
				

				break;
			}
			else
				continue;
		}
	}

	for (int l = 1; l < dtp + 1; l++)
	{
		int ds2 = 0;
		int u, v;
		

		for (int dr = 1; dr < drs + 1; dr++)
		{
			PointNum2[dr] = int((200 + (dl / 2) + dl * (dr - 1)) / ds);
			int n4 = l - ds2-1;//for pad in row
			ds2 = ds2 + PointNum2[dr];
			if (l - ds2 <= 0)
			{
				u = dr;
				//v = l - (ds2 - PointNum2[dr]);
				v = ds2 - l + 1;
				dyc[l] = 100 + (dl / 2) + dl * (u - 1);
				dzc[l] = dyc[l] - (ds / 2) - ds * (v - 1);
				cout << l << "\t" << "dyc" << "[" << l << "]" << "\t" << dyc[l] << "\t" << "dzc" << "[" << l << "]" << "\t" << dzc[l] << endl;

				dy0[l] = (dyc[l] - (dl / 2)) / 10;
				dy1[l] = (dyc[l] + (dl / 2)) / 10;
				dy2[l] = (dyc[l] + (dl / 2)) / 10;
				dy3[l] = (dyc[l] - (dl / 2)) / 10;

				dz0[l] = (dzc[l] - (ds / 2) - ((dl / 2) * ((dzc[l] - (ds / 2)) / dyc[l])) - 350) / 10;
				dz1[l] = (dzc[l] - (ds / 2) + ((dl / 2) * ((dzc[l] - (ds / 2)) / dyc[l])) - 350) / 10;
				dz2[l] = (dzc[l] + (ds / 2) + ((dl / 2) * ((dzc[l] + (ds / 2)) / dyc[l])) - 350) / 10;
				dz3[l] = (dzc[l] + (ds / 2) - ((dl / 2) * ((dzc[l] + (ds / 2)) / dyc[l])) - 350) / 10;
				int id6 = l + id5-1;
				double Center4X = (dy0[l] + dy1[l] + dy2[l] + dy3[l]) / 4;
				double Center4Z = (dz0[l] + dz1[l] + dz2[l] + dz3[l]) / 4;
				outfile1 << id6 << "\t" << "3" <<"\t"<< dr-1 << setw(10) << n4 << setw(12) << -Center4X << setw(12) << Center4Z << setw(12) << -dy0[l] << setw(12) << dz0[l] << setw(12) << -dy1[l] << setw(12) << dz1[l] << setw(12) << -dy2[l] << setw(12) << dz2[l] << setw(12) << -dy3[l] << setw(12) << dz3[l] << setw(12)<<Center4Z << setw(12) << Center4X << setw(12) << dz0[l] << setw(12) << dy0[l] << setw(12) << dz1[l] << setw(12) << dy1[l] << setw(12) << dz2[l] << setw(12) << -dy2[l] << setw(12) <<dz3[l] << setw(12) << dy3[l] << endl;
				break;
			}
			else
				continue;
		}
	}
	//outfile1 << "PadID" << "\t" << "Sector" << "\t" << "Row" << setw(6) << "Pad" << setw(10) << "x[0]" << setw(12) << "z[0]" << setw(12) << "x[1]" << setw(12) << "z[1]" << setw(12) << "x[2]" << setw(12) << "z[2]" << setw(12) << "x[3]" << setw(12) << "z[3]" << setw(12) << "CenterX" << setw(12) << "CenterZ" << "\t" << "(Local coordinates)" << endl;


	outfile1.close();
	//outfile2.close();
	
}
