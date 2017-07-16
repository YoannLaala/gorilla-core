/******************************************************************************
**	Includes
******************************************************************************/
#include "Matrix44.hpp"
#include <memory.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Math
{
	const Matrix44 Matrix44::Identity = Matrix44();

	//!	@brief		Constructor
	//!	@date		2015-11-08
	Matrix44::Matrix44()
	{
		m_mValue[0][0] = 1.0f;
		m_mValue[1][0] = 0.0f;
		m_mValue[2][0] = 0.0f;
		m_mValue[3][0] = 0.0f;

		m_mValue[0][1] = 0.0f;
		m_mValue[1][1] = 1.0f;
		m_mValue[2][1] = 0.0f;
		m_mValue[3][1] = 0.0f;

		m_mValue[0][2] = 0.0f;
		m_mValue[1][2] = 0.0f;
		m_mValue[2][2] = 1.0f;
		m_mValue[3][2] = 0.0f;

		m_mValue[0][3] = 0.0f;
		m_mValue[1][3] = 0.0f;
		m_mValue[2][3] = 0.0f;
		m_mValue[3][3] = 1.0f;
	}

	//!	@brief		Constructor
	//!	@date		2015-11-08
	Matrix44::Matrix44(const Matrix44& _mOther)
	{
		m_mValue[0][0] = _mOther[0][0];
		m_mValue[1][0] = _mOther[1][0];
		m_mValue[2][0] = _mOther[2][0];
		m_mValue[3][0] = _mOther[3][0];
	
		m_mValue[0][1] = _mOther[0][1];
		m_mValue[1][1] = _mOther[1][1];
		m_mValue[2][1] = _mOther[2][1];
		m_mValue[3][1] = _mOther[3][1];
	
		m_mValue[0][2] = _mOther[0][2];
		m_mValue[1][2] = _mOther[1][2];
		m_mValue[2][2] = _mOther[2][2];
		m_mValue[3][2] = _mOther[3][2];
		
		m_mValue[0][3] = _mOther[0][3];
		m_mValue[1][3] = _mOther[1][3];
		m_mValue[2][3] = _mOther[2][3];
		m_mValue[3][3] = _mOther[3][3];
	}

	//!	@brief		SetRotation
	//!	@date		2015-11-08
	void Matrix44::SetRotation(const Quaternion& _qRotation)
	{
		// Get Axis and angle information
		Math::Vector3 vAxis(2.0f * _qRotation.GetX(), 2.0f * _qRotation.GetY(), 2.0f * _qRotation.GetZ());
		float32 xx = vAxis.GetX() * _qRotation.GetX();
		float32 xy = vAxis.GetX() * _qRotation.GetY();
		float32 xz = vAxis.GetX() * _qRotation.GetZ();
		float32 xw = vAxis.GetX() * _qRotation.GetW();
		float32 yy = vAxis.GetY() * _qRotation.GetY();
		float32 yz = vAxis.GetY() * _qRotation.GetZ();
		float32 yw = vAxis.GetY() * _qRotation.GetW();
		float32 zz = vAxis.GetZ() * _qRotation.GetZ();
		float32 zw = vAxis.GetZ() * _qRotation.GetW();

		// Compute Rotation
		m_mValue[0][0] = 1.0f - yy - zz;
		m_mValue[1][0] = xy - zw;
		m_mValue[2][0] = xz + yw;
		m_mValue[3][0] = 0.0f;	

		m_mValue[0][1] = xy + zw;
		m_mValue[1][1] = 1.0f - xx - zz;
		m_mValue[2][1] = yz - xw;
		m_mValue[3][1] = 0.0f;		

		m_mValue[0][2] = xz - yw;
		m_mValue[1][2] = yz + xw;
		m_mValue[2][2] = 1.0f - xx - yy;
		m_mValue[3][2] = 0.0f;	

		m_mValue[0][3] = 0.0f;
		m_mValue[1][3] = 0.0f;
		m_mValue[2][3] = 0.0f;
		m_mValue[3][3] = 1.0f;
	}

	//!	@brief		Transform
	//!	@details	Combine all transformation
	//!	@date		2015-11-08
	void Matrix44::Transform(const Vector3& _vPosition, const Quaternion& _qRotation, const Vector3& _vScale)
	{
		SetRotation(_qRotation);
		ApplyScale(_vScale);
		ApplyTranslation(_vPosition);
	}

	//!	@brief		Transform
	//!	@date		2015-11-08
	void Matrix44::Transform(const Matrix44& _mOther)
	{
		Matrix44 mThis(*this);

		m_mValue[0][0] =  mThis[0][0] * _mOther[0][0]; 
		m_mValue[0][0] += mThis[0][1] * _mOther[1][0];
		m_mValue[0][0] += mThis[0][2] * _mOther[2][0];
		m_mValue[0][0] += mThis[0][3] * _mOther[3][0];

		m_mValue[0][1] =  mThis[0][0] * _mOther[0][1]; 
		m_mValue[0][1] += mThis[0][1] * _mOther[1][1];
		m_mValue[0][1] += mThis[0][2] * _mOther[2][1];
		m_mValue[0][1] += mThis[0][3] * _mOther[3][1];

		m_mValue[0][2] =  mThis[0][0] * _mOther[0][2]; 
		m_mValue[0][2] += mThis[0][1] * _mOther[1][2];
		m_mValue[0][2] += mThis[0][2] * _mOther[2][2];
		m_mValue[0][2] += mThis[0][3] * _mOther[3][2];

		m_mValue[0][3] =  mThis[0][0] * _mOther[0][3]; 
		m_mValue[0][3] += mThis[0][1] * _mOther[1][3];
		m_mValue[0][3] += mThis[0][2] * _mOther[2][3];
		m_mValue[0][3] += mThis[0][3] * _mOther[3][3];

		m_mValue[1][0] =  mThis[1][0] * _mOther[0][0]; 
		m_mValue[1][0] += mThis[1][1] * _mOther[1][0];
		m_mValue[1][0] += mThis[1][2] * _mOther[2][0];
		m_mValue[1][0] += mThis[1][3] * _mOther[3][0];
			  				 
		m_mValue[1][1] =  mThis[1][0] * _mOther[0][1]; 
		m_mValue[1][1] += mThis[1][1] * _mOther[1][1];
		m_mValue[1][1] += mThis[1][2] * _mOther[2][1];
		m_mValue[1][1] += mThis[1][3] * _mOther[3][1];
			  				 
		m_mValue[1][2] =  mThis[1][0] * _mOther[0][2]; 
		m_mValue[1][2] += mThis[1][1] * _mOther[1][2];
		m_mValue[1][2] += mThis[1][2] * _mOther[2][2];
		m_mValue[1][2] += mThis[1][3] * _mOther[3][2];
			  				 
		m_mValue[1][3] =  mThis[1][0] * _mOther[0][3]; 
		m_mValue[1][3] += mThis[1][1] * _mOther[1][3];
		m_mValue[1][3] += mThis[1][2] * _mOther[2][3];
		m_mValue[1][3] += mThis[1][3] * _mOther[3][3];

		m_mValue[2][0] =  mThis[2][0] * _mOther[0][0]; 
		m_mValue[2][0] += mThis[2][1] * _mOther[1][0];
		m_mValue[2][0] += mThis[2][2] * _mOther[2][0];
		m_mValue[2][0] += mThis[2][3] * _mOther[3][0];
			  		 	 
		m_mValue[2][1] =  mThis[2][0] * _mOther[0][1]; 
		m_mValue[2][1] += mThis[2][1] * _mOther[1][1];
		m_mValue[2][1] += mThis[2][2] * _mOther[2][1];
		m_mValue[2][1] += mThis[2][3] * _mOther[3][1];
			  			 	 
		m_mValue[2][2] =  mThis[2][0] * _mOther[0][2]; 
		m_mValue[2][2] += mThis[2][1] * _mOther[1][2];
		m_mValue[2][2] += mThis[2][2] * _mOther[2][2];
		m_mValue[2][2] += mThis[2][3] * _mOther[3][2];
			  			 	 
		m_mValue[2][3] =  mThis[2][0] * _mOther[0][3]; 
		m_mValue[2][3] += mThis[2][1] * _mOther[1][3];
		m_mValue[2][3] += mThis[2][2] * _mOther[2][3];
		m_mValue[2][3] += mThis[2][3] * _mOther[3][3];

		m_mValue[3][0] =  mThis[3][0] * _mOther[0][0]; 
		m_mValue[3][0] += mThis[3][1] * _mOther[1][0];
		m_mValue[3][0] += mThis[3][2] * _mOther[2][0];
		m_mValue[3][0] += mThis[3][3] * _mOther[3][0];
			   	 			 
		m_mValue[3][1] =  mThis[3][0] * _mOther[0][1]; 
		m_mValue[3][1] += mThis[3][1] * _mOther[1][1];
		m_mValue[3][1] += mThis[3][2] * _mOther[2][1];
		m_mValue[3][1] += mThis[3][3] * _mOther[3][1];
			  		 	 	 
		m_mValue[3][2] =  mThis[3][0] * _mOther[0][2]; 
		m_mValue[3][2] += mThis[3][1] * _mOther[1][2];
		m_mValue[3][2] += mThis[3][2] * _mOther[2][2];
		m_mValue[3][2] += mThis[3][3] * _mOther[3][2];
			  		 	 	 
		m_mValue[3][3] =  mThis[3][0] * _mOther[0][3]; 
		m_mValue[3][3] += mThis[3][1] * _mOther[1][3];
		m_mValue[3][3] += mThis[3][2] * _mOther[2][3];
		m_mValue[3][3] += mThis[3][3] * _mOther[3][3];
	}

	Matrix44 Matrix44::Inverse()
	{
		Matrix44 mInverse;
		mInverse[0][0] = m_mValue[1][1] * m_mValue[2][2] * m_mValue[3][3] - 
						 m_mValue[1][1] * m_mValue[2][3] * m_mValue[3][2] - 
						 m_mValue[2][1] * m_mValue[1][2] * m_mValue[3][3] + 
						 m_mValue[2][1] * m_mValue[1][3] * m_mValue[3][2] +
						 m_mValue[3][1] * m_mValue[1][2] * m_mValue[2][3] - 
						 m_mValue[3][1] * m_mValue[1][3] * m_mValue[2][2];

		 mInverse[1][0] = -m_mValue[1][0]  * m_mValue[2][2] * m_mValue[3][3] + 
				  m_mValue[1][0]  * m_mValue[2][3] * m_mValue[3][2] + 
				  m_mValue[2][0]  * m_mValue[1][2]  * m_mValue[3][3] - 
				  m_mValue[2][0]  * m_mValue[1][3]  * m_mValue[3][2] - 
				  m_mValue[3][0] * m_mValue[1][2]  * m_mValue[2][3] + 
				  m_mValue[3][0] * m_mValue[1][3]  * m_mValue[2][2];

		mInverse[2][0] = m_mValue[1][0]  * m_mValue[2][1] * m_mValue[3][3] - 
				 m_mValue[1][0]  * m_mValue[2][3] * m_mValue[3][1] - 
				 m_mValue[2][0]  * m_mValue[1][1] * m_mValue[3][3] + 
				 m_mValue[2][0]  * m_mValue[1][3] * m_mValue[3][1] + 
				 m_mValue[3][0] * m_mValue[1][1] * m_mValue[2][3] - 
				 m_mValue[3][0] * m_mValue[1][3] * m_mValue[2][1];

		mInverse[3][0] = -m_mValue[1][0]  * m_mValue[2][1] * m_mValue[3][2] + 
				   m_mValue[1][0]  * m_mValue[2][2] * m_mValue[3][1] +
				   m_mValue[2][0]  * m_mValue[1][1] * m_mValue[3][2] - 
				   m_mValue[2][0]  * m_mValue[1][2] * m_mValue[3][1] - 
				   m_mValue[3][0] * m_mValue[1][1] * m_mValue[2][2] + 
				   m_mValue[3][0] * m_mValue[1][2] * m_mValue[2][1];

		mInverse[0][1] = -m_mValue[0][1]  * m_mValue[2][2] * m_mValue[3][3] + 
				  m_mValue[0][1]  * m_mValue[2][3] * m_mValue[3][2] + 
				  m_mValue[2][1]  * m_mValue[0][2] * m_mValue[3][3] - 
				  m_mValue[2][1]  * m_mValue[0][3] * m_mValue[3][2] - 
				  m_mValue[3][1] * m_mValue[0][2] * m_mValue[2][3] + 
				  m_mValue[3][1] * m_mValue[0][3] * m_mValue[2][2];

		mInverse[1][1] = m_mValue[0][0]  * m_mValue[2][2] * m_mValue[3][3] - 
				 m_mValue[0][0]  * m_mValue[2][3] * m_mValue[3][2] - 
				 m_mValue[2][0]  * m_mValue[0][2] * m_mValue[3][3] + 
				 m_mValue[2][0]  * m_mValue[0][3] * m_mValue[3][2] + 
				 m_mValue[3][0] * m_mValue[0][2] * m_mValue[2][3] - 
				 m_mValue[3][0] * m_mValue[0][3] * m_mValue[2][2];

		 mInverse[2][1] = -m_mValue[0][0]  * m_mValue[2][1] * m_mValue[3][3] + 
				  m_mValue[0][0]  * m_mValue[2][3] * m_mValue[3][1] + 
				  m_mValue[2][0]  * m_mValue[0][1] * m_mValue[3][3] - 
				  m_mValue[2][0]  * m_mValue[0][3] * m_mValue[3][1] - 
				  m_mValue[3][0] * m_mValue[0][1] * m_mValue[2][3] + 
				  m_mValue[3][0] * m_mValue[0][3] * m_mValue[2][1];

		 mInverse[3][1] = m_mValue[0][0]  * m_mValue[2][1] * m_mValue[3][2] - 
				  m_mValue[0][0]  * m_mValue[2][2] * m_mValue[3][1] - 
				  m_mValue[2][0]  * m_mValue[0][1] * m_mValue[3][2] + 
				  m_mValue[2][0]  * m_mValue[0][2] * m_mValue[3][1] + 
				  m_mValue[3][0] * m_mValue[0][1] * m_mValue[2][2] - 
				  m_mValue[3][0] * m_mValue[0][2] * m_mValue[2][1];

		mInverse[0][2] = m_mValue[0][1]  * m_mValue[1][2] * m_mValue[3][3] - 
				 m_mValue[0][1]  * m_mValue[1][3] * m_mValue[3][2] - 
				 m_mValue[1][1]  * m_mValue[0][2] * m_mValue[3][3] + 
				 m_mValue[1][1]  * m_mValue[0][3] * m_mValue[3][2] + 
				 m_mValue[3][1] * m_mValue[0][2] * m_mValue[1][3] - 
				 m_mValue[3][1] * m_mValue[0][3] * m_mValue[1][2];

		mInverse[1][2] = -m_mValue[0][0]  * m_mValue[1][2] * m_mValue[3][3] + 
				  m_mValue[0][0]  * m_mValue[1][3] * m_mValue[3][2] + 
				  m_mValue[1][0]  * m_mValue[0][2] * m_mValue[3][3] - 
				  m_mValue[1][0]  * m_mValue[0][3] * m_mValue[3][2] - 
				  m_mValue[3][0] * m_mValue[0][2] * m_mValue[1][3] + 
				  m_mValue[3][0] * m_mValue[0][3] * m_mValue[1][2];

		 mInverse[2][2] = m_mValue[0][0]  * m_mValue[1][1] * m_mValue[3][3] - 
				  m_mValue[0][0]  * m_mValue[1][3] * m_mValue[3][1] - 
				  m_mValue[1][0]  * m_mValue[0][1] * m_mValue[3][3] + 
				  m_mValue[1][0]  * m_mValue[0][3] * m_mValue[3][1] + 
				  m_mValue[3][0] * m_mValue[0][1] * m_mValue[1][3] - 
				  m_mValue[3][0] * m_mValue[0][3] * m_mValue[1][1];

		 mInverse[3][2] = -m_mValue[0][0]  * m_mValue[1][1] * m_mValue[3][2] + 
				   m_mValue[0][0]  * m_mValue[1][2] * m_mValue[3][1] + 
				   m_mValue[1][0]  * m_mValue[0][1] * m_mValue[3][2] - 
				   m_mValue[1][0]  * m_mValue[0][2] * m_mValue[3][1] - 
				   m_mValue[3][0] * m_mValue[0][1] * m_mValue[1][2] + 
				   m_mValue[3][0] * m_mValue[0][2] * m_mValue[1][1];

		mInverse[0][3] = -m_mValue[0][1] * m_mValue[1][2] * m_mValue[2][3] + 
				  m_mValue[0][1] * m_mValue[1][3] * m_mValue[2][2] + 
				  m_mValue[1][1] * m_mValue[0][2] * m_mValue[2][3] - 
				  m_mValue[1][1] * m_mValue[0][3] * m_mValue[2][2] - 
				  m_mValue[2][1] * m_mValue[0][2] * m_mValue[1][3] + 
				  m_mValue[2][1] * m_mValue[0][3] * m_mValue[1][2];

		mInverse[1][3] = m_mValue[0][0] * m_mValue[1][2] * m_mValue[2][3] - 
				 m_mValue[0][0] * m_mValue[1][3] * m_mValue[2][2] - 
				 m_mValue[1][0] * m_mValue[0][2] * m_mValue[2][3] + 
				 m_mValue[1][0] * m_mValue[0][3] * m_mValue[2][2] + 
				 m_mValue[2][0] * m_mValue[0][2] * m_mValue[1][3] - 
				 m_mValue[2][0] * m_mValue[0][3] * m_mValue[1][2];

		mInverse[2][3] = -m_mValue[0][0] * m_mValue[1][1] * m_mValue[2][3] + 
				   m_mValue[0][0] * m_mValue[1][3] * m_mValue[2][1] + 
				   m_mValue[1][0] * m_mValue[0][1] * m_mValue[2][3] - 
				   m_mValue[1][0] * m_mValue[0][3] * m_mValue[2][1] - 
				   m_mValue[2][0] * m_mValue[0][1] * m_mValue[1][3] + 
				   m_mValue[2][0] * m_mValue[0][3] * m_mValue[1][1];

		 mInverse[3][3] = m_mValue[0][0] * m_mValue[1][1] * m_mValue[2][2] - 
				  m_mValue[0][0] * m_mValue[1][2] * m_mValue[2][1] - 
				  m_mValue[1][0] * m_mValue[0][1] * m_mValue[2][2] + 
				  m_mValue[1][0] * m_mValue[0][2] * m_mValue[2][1] + 
				  m_mValue[2][0] * m_mValue[0][1] * m_mValue[1][2] - 
				  m_mValue[2][0] * m_mValue[0][2] * m_mValue[1][1];

		float32 fDeterminant = m_mValue[0][0] * mInverse[0][0] + m_mValue[0][1] * mInverse[1][0] + m_mValue[0][2] * mInverse[2][0] + m_mValue[0][3] * mInverse[3][0];
		if (fDeterminant == 0)
		{
			return Matrix44::Identity;
		}
        

		fDeterminant = 1.0f / fDeterminant;
		float32* pValue = mInverse[0];
		for (uint8 uiValue = 0; uiValue < 16; ++uiValue)
			pValue[uiValue] *= fDeterminant;

		return mInverse;
	}
}}