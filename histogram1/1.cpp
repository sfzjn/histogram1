#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace std;
using namespace cv;

void main()
{
	Mat image,imagegray,image1,imagecv;
	int his[256]={0},grayhis[256]={0},colorhis[512]={0},i,j,type,depth,sum,n,tmpa;
	float grayp[256]={0},colorHSL[1000][1000][3]={0},colorBGR[1000][1000][3]={0},l,colorp[512]={0},max,min,q,p,hk,tBGR[3]={0},tR,tG,tB,tC;
	/*image=imread("C:\\fl.bmp");      //灰度均衡
	/ *i=image.rows;
	j=image.cols;* /
	sum=image.rows*image.cols;
	for (i=0;i<image.rows;i++)
	{
		for (j=0;j<image.cols;j++)
		{
			his[image.at<uchar>(i,j)]++;
			
		}
	}
	grayp[0]=(float)his[0]/sum;
	grayhis[0]=grayp[0]*256;
	for (i=1;i<256;i++)
	{
		grayp[i]=(float)his[i]/sum+grayp[i-1];
		grayhis[i]=grayp[i]*256;
	}
	image1=image.clone();
	cvtColor(image,image1,CV_BGR2GRAY);
	for (i=0;i<image1.rows;i++)
	{
		for (j=0;j<image1.cols;j++)
		{
			image1.at<uchar>(i,j)=grayhis[image1.at<uchar>(i,j)];
		}
	}
	type=image.type();
	depth=image.depth();
	cvtColor(image,imagegray,CV_BGR2GRAY);
	type=imagegray.type();
	depth=imagegray.depth();
	equalizeHist(imagegray,imagecv);
	for (i=0;i<imagecv.rows;i++)
	{
		for (j=0;j<imagecv.cols;j++)
		{
			his[imagecv.at<uchar>(i,j)]++;
		}
	}

	imshow("原图",image);
	imshow("灰度图",imagegray);
	imshow("自己均衡图",image1);
	imshow("OpenCV均衡",imagecv);*/
	image=imread("c:\\ldr.bmp");
	sum=image.rows*image.cols;
	for (i=0;i<image.rows;i++) //rows:一共多少行
	{
		for (j=0;j<image.cols;j++)	//cols:一共多少行
		{
			colorBGR[i][j][0]=(float)image.at<Vec3b>(i,j)[0];
			colorBGR[i][j][1]=(float)image.at<Vec3b>(i,j)[1];
			colorBGR[i][j][2]=(float)image.at<Vec3b>(i,j)[2];
			max=colorBGR[i][j][0];
			min=colorBGR[i][j][0];
			if (max<colorBGR[i][j][1])
			{
				max=colorBGR[i][j][1];
				if (max<colorBGR[i][j][2])
				{
					max=colorBGR[i][j][2];
				}
			}
			else
				if (max<colorBGR[i][j][2])
				{
					max=colorBGR[i][j][2];
				}
			if (min>colorBGR[i][j][1])
			{
				min=colorBGR[i][j][1];
				if (min>colorBGR[i][j][2])
				{
					min=colorBGR[i][j][2];
				}
			}
			else
				if (min>colorBGR[i][j][2])
				{
					min=colorBGR[i][j][2];
				}
			//max=max;
			//min=min;
			if (max==min)
			{
				colorHSL[i][j][0]=0;
			}
			else
				if (max==colorBGR[i][j][2]&&((colorBGR[i][j][1]>colorBGR[i][j][0])||(colorBGR[i][j][1]==colorBGR[i][j][0])))
				{
					colorHSL[i][j][0]=60*(colorBGR[i][j][1]-colorBGR[i][j][0])/(max-min);
				}
				else
					if (max==colorBGR[i][j][2]&&colorBGR[i][j][1]<colorBGR[i][j][0])
					{
						colorHSL[i][j][0]=60*(colorBGR[i][j][1]-colorBGR[i][j][0])/(max-min)+360;
					}
					else
						if (max==colorBGR[i][j][1])
						{
							colorHSL[i][j][0]=60*(colorBGR[i][j][0]-colorBGR[i][j][2])/(max-min)+120;
						}
						else
							if (max==colorBGR[i][j][0])
							{
								colorHSL[i][j][0]=60*(colorBGR[i][j][2]-colorBGR[i][j][1])/(max-min)+240;
							}
			colorHSL[i][j][2]=(max+min);
			if (colorHSL[i][j][2]==0||max==min)
			{
				colorHSL[i][j][1]=0;
			}
			else
				if ((colorHSL[i][j][2]/512)>0.5)
				{
					colorHSL[i][j][1]=(max-min)/(2*256-max-min);
				}
				else
					colorHSL[i][j][1]=(max-min)/(max+min);
		}
	}
	for (i=0;i<image.rows;i++)
	{
		for (j=0;j<image.cols;j++)
		{
			colorhis[(int)colorHSL[i][j][2]]++;
		}
	}
	colorp[0]=(float)colorhis[0]/sum;
	for (i=1;i<512;i++)
	{
			colorp[i]=colorp[i-1]+(float)colorhis[i]/sum;
	}
	for (i=0;i<image.rows;i++)
	{
		for (j=0;j<image.cols;j++)
		{
			colorHSL[i][j][2]=colorp[(int)colorHSL[i][j][2]]*512;
			l=colorHSL[i][j][2]/512;
			if (l<0.5)
			{
				q=l*(1+colorHSL[i][j][1]);
			}
			else
			{
				q=l+colorHSL[i][j][1]-(l*colorHSL[i][j][1]);
			}
			p=2*l-q;
			hk=colorHSL[i][j][0]/360;
			tBGR[0]=hk-1.0/3;
			tBGR[1]=hk;
			tBGR[2]=hk+1.0/3;
			for(n=0;n<3;n++)
			{
				if (tBGR[n]<0)
				{
					tBGR[n]++;
				}
				else
					if (tBGR[n]>1)
					{
						tBGR[n]--;
					}
				if (tBGR[n]<1.0/6)
				{
					colorBGR[i][j][n]=p+((q-p)*6*tBGR[n]);
				}
				else
					if (tBGR[n]<1.0/2)
					{
						colorBGR[i][j][n]=q;
					}
					else
						if (tBGR[n]<2.0/3)
						{
							colorBGR[i][j][n]=p+((q-p)*6*(2.0/3-tBGR[n]));
						}
						else
							colorBGR[i][j][n]=p;
			}
			image.at<Vec3b>(i,j)[0]=colorBGR[i][j][0]*255;
			image.at<Vec3b>(i,j)[1]=colorBGR[i][j][1]*255;
			image.at<Vec3b>(i,j)[2]=colorBGR[i][j][2]*255;
			tmpa=image.at<Vec3b>(i,j)[2];
			//colorhis[(int)colorHSL[i][j][2]]++;
		}
	}
	imshow("彩色均衡",image);
	imwrite("c:\\彩色均衡后.bmp",image);
	
	waitKey(0);
}
