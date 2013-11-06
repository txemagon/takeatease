/*
 * Copyright (c) 2013  txemagon / imasen
 *                   txema.gonz@gmail.com
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


#ifndef _FreqFilter_
#define _FreqFilter_

#include "CLAM/InPort.hxx"
#include "CLAM/OutPort.hxx"
#include "CLAM/InControl.hxx"
#include "CLAM/Frame.hxx"
#include "CLAM/FrameTransformation.hxx"
#include "CLAM/FrameTransformationConfig.hxx"

namespace CLAM{

	class FreqFilter: public FrameTransformationTmpl<Spectrum>
	{
		InPort<Spectrum> mIn;
		OutPort<Spectrum> mOut;


		FloatInControl mShiftAmount;
	public:
		const char* GetClassName() const
		{
			return "FreqFilter";
		}

		FreqFilter() 
			: 
			mIn("In Spectrum", this),
			mOut("Out Spectrum", this),
			mShiftAmount("Shift Amount", this)
		{
			Configure( FrameTransformationConfig() );
		}

 		~FreqFilter() {}	
		
		bool Do(const Frame& in, Frame& out)
		{
			return Do(in.GetSpectrum(), 
				  out.GetSpectrum());
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
		DataArray mOMagArray;
		DataArray mOPhaseArray;
	
	};		
};//namespace CLAM

#endif // _FreqFilter_

