module;

export module Engine;

import EngineTypes;
import AhtomWindow;
import AhtomInstance;
import AhtomDevice;
import AhtomSwapChain;
import AhtomRenderPass;
import AhtomDescriptor;
import AhtomGraphicsPipeline;

constexpr char APP_NAME[] = "AhTomEngine";

export class AhtomEngine
{
public:
	AhtomEngine(int majorV, int minorV, int patchV);
	~AhtomEngine();

	void run();

private:

	Version mVersion;
	AhtomInstance* mInstance = nullptr;
	AhtomWindow* mWindow = nullptr;
	AhtomDevice* mAhtomDevice = nullptr;
	AhtomSwapChain* mSwapChain = nullptr;
	AhtomRenderPass* mRenderPass = nullptr;
	AhtomDescriptor* mDescriptor = nullptr;
	AhtomGraphicsPipeline* mGraphicsPipeline = nullptr;
	

	void cleanup();
	void initVulkan();
	void initWindow();
};

AhtomEngine::AhtomEngine(int majorV, int minorV, int patchV) : mVersion({majorV, minorV, patchV})
{
}

AhtomEngine::~AhtomEngine()
{
	cleanup();
}

void AhtomEngine::run()
{
	initWindow();
	initVulkan();

}


void AhtomEngine::cleanup()
{
}

void AhtomEngine::initVulkan()
{
	mInstance = new AhtomInstance(APP_NAME, mVersion);

	mWindow->createSurface(mInstance->getVkInstance());

	mAhtomDevice = new AhtomDevice(mInstance->getVkInstance(), mWindow->getSurface());

	mAhtomDevice->createLogicalDevice(mWindow->getSurface());

	mSwapChain = new AhtomSwapChain(*mAhtomDevice, mWindow->getSurface());

	mSwapChain->createImageViews(*mAhtomDevice);

	mRenderPass = new AhtomRenderPass(*mAhtomDevice, *mSwapChain);

	mDescriptor = new AhtomDescriptor(*mAhtomDevice);

	mGraphicsPipeline = new AhtomGraphicsPipeline(*mAhtomDevice, *mSwapChain, *mDescriptor, *mRenderPass);
}



void AhtomEngine::initWindow()
{
	mWindow = new AhtomWindow(APP_NAME, mVersion);
}
