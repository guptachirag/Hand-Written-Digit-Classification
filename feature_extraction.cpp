/** This file contain code to read images and corresponding lables from the MNIST DATABASE ,store them in the arrays : tr_data , tr_lables and extract features **/

#include "commonfile.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Function Declarations
int reverseInt (int i);
vector<int> row_histogram(vector< vector<int> > img,int n_rows,int n_cols);
vector<int> col_histogram(vector< vector<int> > img,int n_rows,int n_cols);
vector< vector<int> > cal_1010_bitmap(vector< vector<int> > img,vector<int> hist_row,vector<int> hist_col,int n_rows,int n_cols);
vector<float> feature_extractor(vector< vector<int> > b);
float number_setpixels(vector< vector<int> > b,int r,int c);

/*--------------------------------------------------------------------------------------------------------------------*/
	
//Function Definations

vector< vector<float> >  read_images_extract_features(char filename[],int n_images)
{
    ifstream data(filename,ios::in | ios::binary);

    if (data.is_open())
    {
        int magic_number,number_images,n_rows,n_cols;
        // Start reading Database
        data.read((char*)&magic_number,sizeof(magic_number));
        magic_number= reverseInt(magic_number);
        data.read((char*)&number_images,sizeof(number_images));
        number_images= reverseInt(number_images);
        data.read((char*)&n_rows,sizeof(n_rows));
        n_rows= reverseInt(n_rows);
        data.read((char*)&n_cols,sizeof(n_cols));
        n_cols= reverseInt(n_cols);
		
        //cout<<"magic_number="<<magic_number<<endl;
        //cout<<"n_images="<<n_images<<endl;
        //cout<<"n_rows="<<n_rows<<endl;
        //cout<<"n_cols="<<n_cols<<endl;

        vector< vector<int> > img(n_rows,vector<int>(n_cols));
		vector< vector<float> > db_features(n_images,vector<float>(n_features));
		
        /** Reading each images into img 2D vector**/
        for(int i=0; i<n_images; ++i)
        {
            for(int r=0; r<n_rows; ++r)
            {
                for(int c=0; c<n_cols; ++c)
                {
                    unsigned char temp=0;
                    data.read((char*)&temp,sizeof(temp));
                    img[r][c]=(int)temp;
                    if(img[r][c]<128) img[r][c]=0;
                    else img[r][c]=1;
                }
            }
			
           // if(i==241 || i==323 || i==388 || i==536 || i==592 || i==598 || i==944) continue;  /** CORRUPT IMAGES **/
				
            vector<int> hist_row(n_rows);
            vector<int> hist_col(n_cols);
			vector< vector<int> > b(10,vector<int>(10));
			
            hist_row = row_histogram(img,n_rows,n_cols);
            hist_col = col_histogram(img,n_rows,n_cols);
			//cout<<"fine1\n";
            b=cal_1010_bitmap(img,hist_row,hist_col,n_rows,n_cols);
			//cout<<"fine3\n";
            db_features[i]=feature_extractor(b);
        }
		return db_features;
    }
    else cout<<"error reading image_database\n";
}

int reverseInt (int i) //reverse a int from high-endian to low-endian
{
    unsigned char c1, c2, c3, c4;

    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;

    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}

vector<int> row_histogram(vector< vector<int> > img,int n_rows,int n_cols)
{
    vector<int> r(n_rows);
    for(int i=0; i<n_rows; i++)
    {
        r[i]=0;
        for(int j=1; j<n_cols; j++)
        {
            if(img[i][j-1]!=img[i][j])
                r[i]++;
        }
        if(r[i]>8)
            r[i]=0;
    }
    return r;
}

vector<int> col_histogram(vector< vector<int> > img,int n_rows,int n_cols)
{
    vector<int> c(n_cols);
    for(int i=0; i<n_cols; i++)
    {
        c[i]=0;
        for(int j=1; j<n_rows; j++)
        {
            if(img[j-1][i]!=img[j][i])
                c[i]++;
        }
        if(c[i]>8)
            c[i]=0;
    }
    return c;
}

vector< vector<int> > cal_1010_bitmap(vector< vector<int> > a,vector<int> r,vector<int> c,int n_rows,int n_cols)
{
    int row=n_rows;
    int col=n_cols;
    int i,j,low,high;
    vector< vector<int> > b(10,vector<int>(10));
	
    for(i=1; i<row; i++)
    {
        if(r[i-1]!=r[i])
        {
            low=i;
            break;
        }
    }
    for(i=row-1; i>0; i--)
    {
        if(r[i]!=r[i-1])
        {
            high=i-1;
            break;
        }
    }
	
    row=high-low+1;
    i=row%10;
    i=i>0?10-i:i;
    row=row+i;
    if(i%2==0)
        low=low-(i/2);
    else
        low=low-(i/2)-1;
    high=high+(i/2);
    for(i=low; i<=high; i++)
    {
        for(j=0; j<col; j++)
            a[i-low][j]=a[i][j];
    }
	
    // for column
    for(i=1; i<col; i++)
    {
        if(c[i-1]!=c[i])
        {
            low=i;
            break;
        }
    }
    for(i=col-1; i>0; i--)
    {
        if(c[i]!=c[i-1])
        {
            high=i-1;
            break;
        }
    }
    col=high-low+1;
    i=col%10;
    i=i>0?10-i:i;
    col=col+i;
    if(i%2==0)
        low=low-(i/2);
    else
        low=low-(i/2)-1;
    high=high+(i/2);
    for(j=low; j<=high; j++)
    {
        for(i=0; i<row; i++)
            a[i][j-low]=a[i][j];
    }
	
    // mapping 10x10 matrix
    int row10=row/10;
    int col10=col/10;
    int x,y;

    for(i=0; i<10; i++)
        for(j=0; j<10; j++)
            b[i][j]=0;

    for(i=0; i<row; i++)
    {
        for(j=0; j<col; j++)
        {
            x=i/row10;
            y=j/col10;
            if(a[i][j]==1)
                b[x][y]+=1;
        }
    }

    //0-1 bitmap
    float thresh=(0.05)*row10*col10;

    for(i=0; i<10; i++)
        for(j=0; j<10; j++)
            if(b[i][j]>thresh)
                b[i][j]=1;
            else
                b[i][j]=0;
			
    return b;
}

vector <float> feature_extractor(vector< vector<int> > b)
{
    int f=0,i,j;
    float cnt,setpix=0;
	vector <float> features;
	
    for(i=0; i<=4; i+=2)
    {
        for(j=0; j<=4; j+=2)
        {
            setpix = number_setpixels(b,i,j);
			features.push_back(setpix);
        }
    }
		
	for(i=0;i<10;++i)
	{
		cnt=0;
		for(j=0;j<10;++j) 
		{
			cnt+=b[j][i];
			features.push_back(cnt);
		}
	}
	
	for(i=0;i<10;++i)
	{
		cnt=0;
		for(j=0;j<10;++j)
		{		
			cnt+=b[i][j];
			features.push_back(cnt);
		}
	}
	
	float max_brow_cnt=0,max_brow=0,max_bcol_cnt=0,max_bcol=0;
	for(i=0;i<10;++i)
	{
		cnt=0;
		for(j=0;j<10;++j) cnt+=b[i][j];
		if(max_brow_cnt < cnt)
		{
			max_brow_cnt=cnt;
			max_brow=i;
		}
		
	}
	for(i=0;i<10;++i)
	{
		cnt=0;
		for(j=0;j<10;++j) cnt+=b[j][i];
		if(max_bcol_cnt < cnt)
		{
			max_bcol_cnt=cnt;
			max_bcol=i;
		}
	}
	features.push_back(max_brow);
	features.push_back(max_bcol);
	features.push_back(max_brow_cnt);
	features.push_back(max_bcol_cnt);
	
	cnt=0;
	for(i=3;i<=4;++i)
	{
		for(j=3;j<=4;++j)
		{
			cnt+=b[i][j];
		}
	}
	cnt=cnt>0?1:0;
	features.push_back(cnt);
	
	return features;
}

float number_setpixels(vector< vector<int> > b,int r,int c)
{
    int i,j;
	float ret=0;
    for(i=0;i<4;++i)
    {
        for(j=0;j<4;++j)
        {
            ret+=(float)b[i+r][j+c];
        }
    }
    return ret;
}

vector<int> read_lables(char filename[],int n_images)
{
    ifstream lables(filename,ios::in | ios::binary);

    if (lables.is_open())
    {
        int magic_number,number_images;
        unsigned char lable;
		vector<int> db_lable(n_images);
			
        // Start reading Database
        lables.read((char*)&magic_number,sizeof(magic_number));
        magic_number= reverseInt(magic_number);
        //cout<<magic_number<<"\n";
        lables.read((char*)&number_images,sizeof(number_images));
        number_images= reverseInt(number_images);
        //cout<<number_images<<"\n";

        for(int i=0;i<n_images;++i)
        {
            lables.read((char*)&lable,sizeof(lable));
            db_lable[i]=(int)lable;
            //cout<<(int)lable<<endl;
        }
		return db_lable;
    }
    else cout<<"error reading lables_database\n";
}
