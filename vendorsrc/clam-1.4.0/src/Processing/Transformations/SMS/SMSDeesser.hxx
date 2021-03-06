/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */


#ifndef _SMSDeesser_
#define _SMSDeesser_

#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "Frame.hxx"
#include "Spectrum.hxx"
#include "SegmentTransformationConfig.hxx"
#include "FrameTransformation.hxx"
#include "FrameTransformationConfig.hxx"

#include "Deesser.hxx"

namespace CLAM{


	class SMSDeesser: public FrameTransformation
	{
		
		const char *GetClassName() const {return "SMSDeesser";}

		InPort<Spectrum> mIn;
		OutPort<Spectrum> mOut;
		
		FloatInControl mLowCutoffFreqCtl;
		FloatInControl mHighCutoffFreqCtl;
		FloatInControl mThresholdCtl;
	public:

		SMSDeesser() 
			: 
			mIn("In Spectrum", this), 
			mOut("Out Spectrum", this),
			mLowCutoffFreqCtl("LowCutoff", this),
			mHighCutoffFreqCtl("HighCutoff", this),
			mThresholdCtl("Threshold", this)
		{
			Configure( SegmentTransformationConfig() );
		}

 		~SMSDeesser() {}

		bool Do(const Frame& in, Frame& out)
		{
			return Do(in.GetResidualSpec(), out.GetResidualSpec());
		}

		bool Do(const Spectrum& in, Spectrum& out);

		bool Do()
		{
			bool result = Do(mIn.GetData(), mOut.GetData());
			mIn.Consume();
			mOut.Produce();
			return result;
		}
	private:
		Deesser mDeesser;
	
	};		
} //namespace CLAM

#endif // _SMSDeesser_

