#ifndef TONIC_TESTS_H
#define TONIC_TESTS_H
#include "gtest/gtest.h"
#include "Tonic.h"

using namespace Tonic;

class TonicTests : public testing::Test {
	protected:
		TonicTests();
		~TonicTests();

		void configureStereo( bool stereo );
		void verifyFixedOutputEquals( float expectedOutput );
		void verifyStereoFixedOutputEqualsLeft( float l, float r );
		void verifyBufferFillerMonoFixedOutputEquals( float expectedOutput );
		void verifyBufferFillerStereoFixedOutputEqualsLeft( float l, float r );

		TonicFrames testFrames;
  		Tonic_::SynthesisContext_ testContext;
		float * stereoOutBuffer;
		float * monoOutBuffer;

};

#endif // TONIC_TESTS_H