#include"MNN_Facealignment.h"

using namespace MNN;
using namespace MNN::CV;

static MNN::ScheduleConfig config;
static std::shared_ptr<MNN::Interpreter> mnnNet = nullptr;
static MNN::Session* session = nullptr;
static int TENSOR_SIZE = 0;
static int STD_MODE = 0;
static float* outData = NULL;
static int FA_NUMBER = 101;
int f_MNN_FA_Initialize(char* modelPath, int tensorSize, int stdMode, int numThread)
{
	int ret = 0;
	TENSOR_SIZE = tensorSize;
	config.type = MNN_FORWARD_CPU;
	config.numThread = numThread;
	mnnNet = std::shared_ptr<MNN::Interpreter>(MNN::Interpreter::createFromFile(modelPath));// "C://Users//Administrator//Documents//Visual Studio 2015//Projects//MNN_PC_DEMO//MNN//model//autobc1.mnn"));
	if (mnnNet == NULL)
		return 100;
	session = mnnNet->createSession(config);
	if (session != NULL)
		return ret;
	else
		return 200;

};
void f_MNN_FA_Unitialize()
{
	if (outData != NULL)
		free(outData);
}
int MNN_FA_Process(unsigned char* srcData, int width, int height, int stride, float facePoints[202])
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
	trans.postScale((float)TENSOR_SIZE / (width), (float)TENSOR_SIZE / (height));
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
	for (int i = 0; i < FA_NUMBER * 2; i++)
	{
		facePoints[i] = data[i];
	}
	return ret;
};

