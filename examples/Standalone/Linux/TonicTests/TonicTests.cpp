#include "TonicTests.h"

#define kTestOutputBlockSize kSynthesisBlockSize*4

class TestControlChangeSubscriber : public ControlChangeSubscriber {
    public:
    bool valueChangedFlag;
    TestControlChangeSubscriber() : valueChangedFlag(false){}
    void valueChanged(string, TonicFloat){
        valueChangedFlag = true;
    }
};

TonicTests::TonicTests() {
    configureStereo(false);
    stereoOutBuffer = new float[kTestOutputBlockSize * 2];
    monoOutBuffer = new float[kTestOutputBlockSize];
}

TonicTests::~TonicTests() {
    delete [] stereoOutBuffer;
    delete [] monoOutBuffer;
}

void TonicTests::configureStereo( bool stereo ) {
  testFrames.resize(kSynthesisBlockSize, stereo ? 2 : 1, 0);
}

void TonicTests::verifyFixedOutputEquals( float expectedOutput ) {
  for (unsigned int i=0; i<testFrames.size(); i++){
        ASSERT_EQ(expectedOutput, testFrames[i]) << "Did not produce expected output";
        if (testFrames[i] != expectedOutput) break;
    }
}

void TonicTests::verifyStereoFixedOutputEqualsLeft( float l, float r ) {
    for (unsigned int i=0; i<testFrames.frames(); i++){
    ASSERT_EQ(l, testFrames[2*i]) << "Left channel not produce expected output on frame " << i;
    if (testFrames[2*i] != l) break;

    ASSERT_EQ(r, testFrames[2*i+1]) << "Right channel did not produce expected output on frame " << i;
    if (testFrames[2*i+1] != r) break;
    }
}

void TonicTests::verifyBufferFillerMonoFixedOutputEquals( float expectedOutput ) {
    for (unsigned int i=0; i<kTestOutputBlockSize; i++){
        ASSERT_EQ(expectedOutput, monoOutBuffer[i]) << "Did not produce expected output";
        if (monoOutBuffer[i] != expectedOutput) break;
    }
}

void TonicTests::verifyBufferFillerStereoFixedOutputEqualsLeft( float l, float r ) {
    for (unsigned int i=0; i<kTestOutputBlockSize; i++){
        ASSERT_EQ(l, stereoOutBuffer[2*i]) << "Left channel not produce expected output";
        if (stereoOutBuffer[2*i] != l) break;

        ASSERT_EQ(r, stereoOutBuffer[2*i+1]) << "Right channel did not produce expected output";
        if (stereoOutBuffer[2*i+1] != r) break;
    }
}

TEST_F(TonicTests, fixedValue) {
    // See if FixedValue is correctly filled with a float
  
    configureStereo(false);

    float testFloat = 0.533;
    FixedValue testVal = FixedValue(testFloat);

    testVal.tick(testFrames, testContext);

    verifyFixedOutputEquals(testFloat);
  
}

TEST_F(TonicTests,tickMonoToStereo) {
    // Tick a mono source to a stereo frames object. Ensure both channels equal.

    configureStereo(true);

    float testFloat = 0.533;
    FixedValue testVal = FixedValue(testFloat);

    testVal.tick(testFrames, testContext);

    ASSERT_EQ(testFrames.channels(), (unsigned int)2) << "TestFrames should have 2 channels";

    verifyFixedOutputEquals(testFloat);
}

