// MNN.cpp : 定义控制台应用程序的入口点。
//

#include"MNN.h"

using namespace MNN;
using namespace MNN::CV;

static MNN::ScheduleConfig config;
static std::shared_ptr<MNN::Interpreter> mnnNet = nullptr;
static MNN::Session* session = nullptr;
static int TENSOR_SIZE = 0;
static int STD_MODE = 0;
static unsigned char* outData = NULL;
static MNN::ScheduleConfig faconfig;
static std::shared_ptr<MNN::Interpreter> famnnNet = nullptr;
static MNN::Session* fasession = nullptr;
static int TENSOR_SIZE_FACE = 96;
int f_MNN_Initialize(char* fdmodelPath, char* famodelPath, int tensorSize, int stdMode, int numThread)
{	
	int ret = 0;
	TENSOR_SIZE = tensorSize;
	config.type = MNN_FORWARD_CPU;
	config.numThread = numThread;
	mnnNet = std::shared_ptr<MNN::Interpreter>(MNN::Interpreter::createFromFile(fdmodelPath));// "C://Users//Administrator//Documents//Visual Studio 2015//Projects//MNN_PC_DEMO//MNN//model//autobc1.mnn"));
	outData = (unsigned char*)malloc(sizeof(unsigned char) * TENSOR_SIZE * TENSOR_SIZE * 4);
	if (outData == NULL)
		return 201;
	if (mnnNet == NULL)
		return 100;
	session = mnnNet->createSession(config);
	faconfig.type = MNN_FORWARD_CPU;
	faconfig.numThread = numThread;
	famnnNet = std::shared_ptr<MNN::Interpreter>(MNN::Interpreter::createFromFile(famodelPath));// "C://Users//Administrator//Documents//Visual Studio 2015//Projects//MNN_PC_DEMO//MNN//model//autobc1.mnn"));
	if (famnnNet == NULL)
		return 100;
	fasession = famnnNet->createSession(faconfig);
	if (session != NULL && fasession != NULL)
		return ret;
	else
		return 200;
	
};
void f_MNN_Unitialize()
{
	if (outData != NULL)
		free(outData);
}
int MNN_Process(unsigned char* srcData, int width, int height, int stride)
{
	int ret = 0;
	unsigned char* pSrc = srcData;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			int temp = pSrc[0];
			pSrc[0] = pSrc[2];
			pSrc[2] = temp;
			pSrc += 4;
		}
	}
	auto input = mnnNet->getSessionInput(session, nullptr);
	CV::ImageProcess::Config preProcessConfig;

	if (STD_MODE == 0)
	{
		const float means[3] = { 0.0f, 0.0f, 0.0f };
		const float norms[3] = { 1.0f / 255.0f,1.0f / 255.0f,1.0f / 255.0f };
		memcpy(preProcessConfig.mean, means, sizeof(means));
		memcpy(preProcessConfig.normal, norms, sizeof(norms));
	}
	else
	{
		const float means[3] = { 127.5f,127.5f,127.5f };
		const float norms[3] = { 0.0078125f,0.0078125f,0.0078125f };
		memcpy(preProcessConfig.mean, means, sizeof(means));
		memcpy(preProcessConfig.normal, norms, sizeof(norms));
	}
	preProcessConfig.sourceFormat = CV::RGBA;
	preProcessConfig.destFormat = CV::RGB;
	preProcessConfig.filterType = CV::BILINEAR;
	auto pretreat = std::shared_ptr<CV::ImageProcess>(CV::ImageProcess::create(preProcessConfig));
	CV::Matrix trans;
	trans.postScale((float)TENSOR_SIZE / (width - 1), (float)TENSOR_SIZE / (height - 1));
	pretreat->setMatrix(trans);
	pSrc = srcData;
	pretreat->convert(pSrc, width, height, 0, input);
	std::vector<int> inputdim;
	inputdim.push_back(1);
	inputdim.push_back(3);
	inputdim.push_back(TENSOR_SIZE);
	inputdim.push_back(TENSOR_SIZE);
	mnnNet->resizeTensor(input, inputdim);
	mnnNet->resizeSession(session);
	mnnNet->runSession(session);
	MNN::Tensor *output = mnnNet->getSessionOutput(session, nullptr);
	auto copy = std::shared_ptr<MNN::Tensor>(MNN::Tensor::createHostTensorFromDevice(output));
	float *data = copy->host<float>();
	pSrc = outData;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			int temp = CLIP3(data[i + j * TENSOR_SIZE] * 255.0f, 0, 255);
			pSrc[2] = temp;
			pSrc[1] = temp;
			pSrc[0] = temp;
			pSrc[3] = 255;
			pSrc += 4;
		}
	}
	return ret;
};

void f_ImagePadding(unsigned char* srcData, int width, int height, int stride, unsigned char* dstData, int dSize)
{
	int x0, y0, index_x0y0, index_x0y1, index_x1y0, index_x1y1;
	float disX, disY, tmp1, tmp2;
	if (width > height)
	{
		int temp = height * dSize / width;
		int offset = (dSize - temp) / 2;
		for (int j = offset; j < dSize - offset; j++)
		{
			for (int i = 0; i < dSize; i++)
			{
				float cx = CLIP3(i * (float)width / (float)dSize, 0, width - 1);
				float cy = CLIP3((j - offset) * (float)height / (float)temp, 0, height - 1);
				x0 = (int)CLIP3(floor(cx), 0, width - 2);
				y0 = (int)CLIP3(floor(cy), 0, height - 2);
				index_x0y0 = (x0 << 2) + y0 * stride;
				index_x1y0 = index_x0y0 + 4;
				index_x0y1 = index_x0y0 + stride;
				index_x1y1 = index_x0y1 + 4;
				disX = cx - x0;
				disY = cy - y0;
				tmp1 = srcData[index_x0y0] + disX *(srcData[index_x1y0] - srcData[index_x0y0]);
				tmp2 = srcData[index_x0y1] + disX *(srcData[index_x1y1] - srcData[index_x0y1]);
				int AB = CLIP3(tmp1 + disY * (tmp2 - tmp1), 0, 255);

				tmp1 = srcData[index_x0y0 + 1] + disX *(srcData[index_x1y0 + 1] - srcData[index_x0y0 + 1]);
				tmp2 = srcData[index_x0y1 + 1] + disX *(srcData[index_x1y1 + 1] - srcData[index_x0y1 + 1]);
				int AG = CLIP3(tmp1 + disY * (tmp2 - tmp1), 0, 255);

				tmp1 = srcData[index_x0y0 + 2] + disX *(srcData[index_x1y0 + 2] - srcData[index_x0y0 + 2]);
				tmp2 = srcData[index_x0y1 + 2] + disX *(srcData[index_x1y1 + 2] - srcData[index_x0y1 + 2]);
				int AR = CLIP3(tmp1 + disY * (tmp2 - tmp1), 0, 255);
				dstData[i * 4 + j * dSize * 4] = AB;
				dstData[i * 4 + j * dSize * 4 + 1] = AG;
				dstData[i * 4 + j * dSize * 4 + 2] = AR;
				dstData[i * 4 + j * dSize * 4 + 3] = 255;
			}
		}
	}
	else
	{
		int temp = width * dSize / height;
		int offset = (dSize - temp) / 2;
		for (int j = 0; j < dSize; j++)
		{
			for (int i = offset; i < dSize - offset; i++)
			{
				float cx = CLIP3((i - offset) * width / temp, 0, width - 1);
				float cy = CLIP3(j * height / dSize, 0, height - 1);
				x0 = (int)CLIP3(floor(cx), 0, width - 2);
				y0 = (int)CLIP3(floor(cy), 0, height - 2);
				index_x0y0 = (x0 << 2) + y0 * stride;
				index_x1y0 = index_x0y0 + 4;
				index_x0y1 = index_x0y0 + stride;
				index_x1y1 = index_x0y1 + 4;
				disX = cx - x0;
				disY = cy - y0;
				tmp1 = srcData[index_x0y0] + disX *(srcData[index_x1y0] - srcData[index_x0y0]);
				tmp2 = srcData[index_x0y1] + disX *(srcData[index_x1y1] - srcData[index_x0y1]);
				int AB = CLIP3(tmp1 + disY * (tmp2 - tmp1), 0, 255);

				tmp1 = srcData[index_x0y0 + 1] + disX *(srcData[index_x1y0 + 1] - srcData[index_x0y0 + 1]);
				tmp2 = srcData[index_x0y1 + 1] + disX *(srcData[index_x1y1 + 1] - srcData[index_x0y1 + 1]);
				int AG = CLIP3(tmp1 + disY * (tmp2 - tmp1), 0, 255);

				tmp1 = srcData[index_x0y0 + 2] + disX *(srcData[index_x1y0 + 2] - srcData[index_x0y0 + 2]);
				tmp2 = srcData[index_x0y1 + 2] + disX *(srcData[index_x1y1 + 2] - srcData[index_x0y1 + 2]);
				int AR = CLIP3(tmp1 + disY * (tmp2 - tmp1), 0, 255);
				dstData[i * 4 + j * dSize * 4] = AB;
				dstData[i * 4 + j * dSize * 4 + 1] = AG;
				dstData[i * 4 + j * dSize * 4 + 2] = AR;
				dstData[i * 4 + j * dSize * 4 + 3] = 255;
			}
		}
	}
}
void f_ImageRePadding(unsigned char* srcData, int width, int height, int stride, unsigned char* dstData, int dSize)
{
	int x0, y0, index_x0y0, index_x0y1, index_x1y0, index_x1y1;
	float disX, disY, tmp1, tmp2;
	unsigned char* pSrc = srcData;
	if (width > height)
	{
		int temp = height * dSize / width;
		int offset = (dSize - temp) / 2;
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				float dx = CLIP3(i * (float)dSize / width, 0, dSize - 1);
				float dy = CLIP3(j * temp / (float)height + offset, 0, dSize - 1);
				x0 = (int)CLIP3(floor(dx), 0, dSize - 2);
				y0 = (int)CLIP3(floor(dy), 0, dSize - 2);
				index_x0y0 = (x0 << 2) + y0 * dSize * 4;
				index_x1y0 = index_x0y0 + 4;
				index_x0y1 = index_x0y0 + dSize * 4;
				index_x1y1 = index_x0y1 + 4;
				disX = dx - x0;
				disY = dy - y0;
				tmp1 = dstData[index_x0y0] + disX *(dstData[index_x1y0] - dstData[index_x0y0]);
				tmp2 = dstData[index_x0y1] + disX *(dstData[index_x1y1] - dstData[index_x0y1]);
				int AB = CLIP3(tmp1 + disY * (tmp2 - tmp1), 0, 255);

				tmp1 = dstData[index_x0y0 + 1] + disX *(dstData[index_x1y0 + 1] - dstData[index_x0y0 + 1]);
				tmp2 = dstData[index_x0y1 + 1] + disX *(dstData[index_x1y1 + 1] - dstData[index_x0y1 + 1]);
				int AG = CLIP3(tmp1 + disY * (tmp2 - tmp1), 0, 255);

				tmp1 = dstData[index_x0y0 + 2] + disX *(dstData[index_x1y0 + 2] - dstData[index_x0y0 + 2]);
				tmp2 = dstData[index_x0y1 + 2] + disX *(dstData[index_x1y1 + 2] - dstData[index_x0y1 + 2]);
				int AR = CLIP3(tmp1 + disY * (tmp2 - tmp1), 0, 255);

				pSrc[0] = AB;
				pSrc[1] = AG;
				pSrc[2] = AR;
				pSrc += 4;
			}
		}
	}
	else
	{
		int temp = width * dSize / height;
		int offset = (dSize - temp) / 2;
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				float dx = CLIP3(i * temp / (float)width + offset, 0, dSize - 1);
				float dy = CLIP3(j * dSize / (float)height, 0, dSize - 1);
				x0 = (int)CLIP3(floor(dx), 0, dSize - 2);
				y0 = (int)CLIP3(floor(dy), 0, dSize - 2);
				index_x0y0 = (x0 << 2) + y0 * dSize * 4;
				index_x1y0 = index_x0y0 + 4;
				index_x0y1 = index_x0y0 + dSize * 4;
				index_x1y1 = index_x0y1 + 4;
				disX = dx - x0;
				disY = dy - y0;
				tmp1 = dstData[index_x0y0] + disX *(dstData[index_x1y0] - dstData[index_x0y0]);
				tmp2 = dstData[index_x0y1] + disX *(dstData[index_x1y1] - dstData[index_x0y1]);
				int AB = CLIP3(tmp1 + disY * (tmp2 - tmp1), 0, 255);

				tmp1 = dstData[index_x0y0 + 1] + disX *(dstData[index_x1y0 + 1] - dstData[index_x0y0 + 1]);
				tmp2 = dstData[index_x0y1 + 1] + disX *(dstData[index_x1y1 + 1] - dstData[index_x0y1 + 1]);
				int AG = CLIP3(tmp1 + disY * (tmp2 - tmp1), 0, 255);

				tmp1 = dstData[index_x0y0 + 2] + disX *(dstData[index_x1y0 + 2] - dstData[index_x0y0 + 2]);
				tmp2 = dstData[index_x0y1 + 2] + disX *(dstData[index_x1y1 + 2] - dstData[index_x0y1 + 2]);
				int AR = CLIP3(tmp1 + disY * (tmp2 - tmp1), 0, 255);

				pSrc[0] = AB;
				pSrc[1] = AG;
				pSrc[2] = AR;
				pSrc += 4;
			}
		}
	}
}
int f_MNN_Process(unsigned char* srcData, int width, int height, int stride)
{
	unsigned char* dstData = (unsigned char*)malloc(sizeof(unsigned char) * TENSOR_SIZE * TENSOR_SIZE * 4);
	f_ImagePadding(srcData, width, height, stride, dstData, TENSOR_SIZE);
	int ret = MNN_Process(dstData, TENSOR_SIZE, TENSOR_SIZE, TENSOR_SIZE * 4);
	f_ImageRePadding(srcData, width, height, stride, outData, TENSOR_SIZE);
	return ret;
}

int f_MNN_FA_Process(unsigned char* srcData, int width, int height, int stride, float facePoints[202])
{
	int ret = 0;
	unsigned char* pSrc = srcData;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			int temp = pSrc[0];
			pSrc[0] = pSrc[2];
			pSrc[2] = temp;
			pSrc += 4;
		}
	}
	auto input = famnnNet->getSessionInput(fasession, nullptr);
	CV::ImageProcess::Config preProcessConfig;

	if (STD_MODE == 0)
	{
		const float means[3] = { 0.0f, 0.0f, 0.0f };
		const float norms[3] = { 1.0f / 255.0f,1.0f / 255.0f,1.0f / 255.0f };
		memcpy(preProcessConfig.mean, means, sizeof(means));
		memcpy(preProcessConfig.normal, norms, sizeof(norms));
	}
	else
	{
		const float means[3] = { 127.5f,127.5f,127.5f };
		const float norms[3] = { 0.0078125f,0.0078125f,0.0078125f };
		memcpy(preProcessConfig.mean, means, sizeof(means));
		memcpy(preProcessConfig.normal, norms, sizeof(norms));
	}
	preProcessConfig.sourceFormat = CV::RGBA;
	preProcessConfig.destFormat = CV::RGB;
	preProcessConfig.filterType = CV::BILINEAR;
	auto pretreat = std::shared_ptr<CV::ImageProcess>(CV::ImageProcess::create(preProcessConfig));
	CV::Matrix trans;
	trans.postScale((float)TENSOR_SIZE_FACE / (width), (float)TENSOR_SIZE_FACE / (height));
	pretreat->setMatrix(trans);
	pSrc = srcData;
	
	std::vector<int> inputdim;
	inputdim.push_back(1);
	inputdim.push_back(3);
	inputdim.push_back(TENSOR_SIZE_FACE);
	inputdim.push_back(TENSOR_SIZE_FACE);
	famnnNet->resizeTensor(input, inputdim);
	famnnNet->resizeSession(fasession);
	pretreat->convert(pSrc, width, height, 0, input);
	famnnNet->runSession(fasession);
	MNN::Tensor *output = famnnNet->getSessionOutput(fasession, nullptr);
	auto copy = std::shared_ptr<MNN::Tensor>(MNN::Tensor::createHostTensorFromDevice(output));
	float *data = copy->host<float>();
	for (int i = 0; i < 101 * 2; i++)
	{
		facePoints[i] = data[i];
	}
	return ret;
};



