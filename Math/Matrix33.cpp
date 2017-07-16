/******************************************************************************
**	Includes
******************************************************************************/
#include "Matrix33.hpp"
#include <memory.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Math
{
	const Matrix33 Matrix33::Identity = Matrix33();

	//!	@brief		Constructor
	//!	@date		2015-11-08
	Matrix33::Matrix33()
	{
		m_aValue[0][0] = 1.0f;
		m_aValue[1][0] = 0.0f;
		m_aValue[2][0] = 0.0f;

		m_aValue[0][1] = 0.0f;
		m_aValue[1][1] = 1.0f;
		m_aValue[2][1] = 0.0f;

		m_aValue[0][2] = 0.0f;
		m_aValue[1][2] = 0.0f;
		m_aValue[2][2] = 1.0f;
	}

	//!	@brief		Constructor
	//!	@date		2015-11-08
	Matrix33::Matrix33(const Matrix33& _mOther)
	{
		m_aValue[0][0] = _mOther[0][0];
		m_aValue[1][0] = _mOther[1][0];
		m_aValue[2][0] = _mOther[2][0];
	
		m_aValue[0][1] = _mOther[0][1];
		m_aValue[1][1] = _mOther[1][1];
		m_aValue[2][1] = _mOther[2][1];
	
		m_aValue[0][2] = _mOther[0][2];
		m_aValue[1][2] = _mOther[1][2];
		m_aValue[2][2] = _mOther[2][2];
	}

	//!	@brief		SetRotation
	//!	@date		2015-11-08
	void Matrix33::SetRotation(const Quaternion& _qRotation)
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
		m_aValue[0][0] = 1.0f - yy - zz;
		m_aValue[1][0] = xy - zw;
		m_aValue[2][0] = xz + yw;
		m_aValue[3][0] = 0.0f;	

		m_aValue[0][1] = xy + zw;
		m_aValue[1][1] = 1.0f - xx - zz;
		m_aValue[2][1] = yz - xw;
		m_aValue[3][1] = 0.0f;		

		m_aValue[0][2] = xz - yw;
		m_aValue[1][2] = yz + xw;
		m_aValue[2][2] = 1.0f - xx - yy;
		m_aValue[3][2] = 0.0f;	

		m_aValue[0][3] = 0.0f;
		m_aValue[1][3] = 0.0f;
		m_aValue[2][3] = 0.0f;
		m_aValue[3][3] = 1.0f;
	}

	//!	@brief		Transform
	//!	@details	Combine all transformation
	//!	@date		2015-11-08
	void Matrix33::Transform(const Vector2& _vPosition, const Quaternion& _qRotation, const Vector2& _vScale)
	{
		SetRotation(_qRotation);
		Scale(_vScale);
		Translate(_vPosition);
	}

	//!	@brief		Transform
	//!	@date		2015-11-08
	void Matrix33::Transform(const Matrix33& _mOther)
	{
		Matrix33 mThis(*this);

		m_aValue[0][0] =  mThis[0][0] * _mOther[0][0]; 
		m_aValue[0][0] += mThis[0][1] * _mOther[1][0];
		m_aValue[0][0] += mThis[0][2] * _mOther[2][0];
		m_aValue[0][0] += mThis[0][3] * _mOther[3][0];

		m_aValue[0][1] =  mThis[0][0] * _mOther[0][1]; 
		m_aValue[0][1] += mThis[0][1] * _mOther[1][1];
		m_aValue[0][1] += mThis[0][2] * _mOther[2][1];
		m_aValue[0][1] += mThis[0][3] * _mOther[3][1];

		m_aValue[0][2] =  mThis[0][0] * _mOther[0][2]; 
		m_aValue[0][2] += mThis[0][1] * _mOther[1][2];
		m_aValue[0][2] += mThis[0][2] * _mOther[2][2];
		m_aValue[0][2] += mThis[0][3] * _mOther[3][2];

		m_aValue[0][3] =  mThis[0][0] * _mOther[0][3]; 
		m_aValue[0][3] += mThis[0][1] * _mOther[1][3];
		m_aValue[0][3] += mThis[0][2] * _mOther[2][3];
		m_aValue[0][3] += mThis[0][3] * _mOther[3][3];

		m_aValue[1][0] =  mThis[1][0] * _mOther[0][0]; 
		m_aValue[1][0] += mThis[1][1] * _mOther[1][0];
		m_aValue[1][0] += mThis[1][2] * _mOther[2][0];
		m_aValue[1][0] += mThis[1][3] * _mOther[3][0];
			  				 
		m_aValue[1][1] =  mThis[1][0] * _mOther[0][1]; 
		m_aValue[1][1] += mThis[1][1] * _mOther[1][1];
		m_aValue[1][1] += mThis[1][2] * _mOther[2][1];
		m_aValue[1][1] += mThis[1][3] * _mOther[3][1];
			  				 
		m_aValue[1][2] =  mThis[1][0] * _mOther[0][2]; 
		m_aValue[1][2] += mThis[1][1] * _mOther[1][2];
		m_aValue[1][2] += mThis[1][2] * _mOther[2][2];
		m_aValue[1][2] += mThis[1][3] * _mOther[3][2];
			  				 
		m_aValue[1][3] =  mThis[1][0] * _mOther[0][3]; 
		m_aValue[1][3] += mThis[1][1] * _mOther[1][3];
		m_aValue[1][3] += mThis[1][2] * _mOther[2][3];
		m_aValue[1][3] += mThis[1][3] * _mOther[3][3];

		m_aValue[2][0] =  mThis[2][0] * _mOther[0][0]; 
		m_aValue[2][0] += mThis[2][1] * _mOther[1][0];
		m_aValue[2][0] += mThis[2][2] * _mOther[2][0];
		m_aValue[2][0] += mThis[2][3] * _mOther[3][0];
			  		 	 
		m_aValue[2][1] =  mThis[2][0] * _mOther[0][1]; 
		m_aValue[2][1] += mThis[2][1] * _mOther[1][1];
		m_aValue[2][1] += mThis[2][2] * _mOther[2][1];
		m_aValue[2][1] += mThis[2][3] * _mOther[3][1];
			  			 	 
		m_aValue[2][2] =  mThis[2][0] * _mOther[0][2]; 
		m_aValue[2][2] += mThis[2][1] * _mOther[1][2];
		m_aValue[2][2] += mThis[2][2] * _mOther[2][2];
		m_aValue[2][2] += mThis[2][3] * _mOther[3][2];
			  			 	 
		m_aValue[2][3] =  mThis[2][0] * _mOther[0][3]; 
		m_aValue[2][3] += mThis[2][1] * _mOther[1][3];
		m_aValue[2][3] += mThis[2][2] * _mOther[2][3];
		m_aValue[2][3] += mThis[2][3] * _mOther[3][3];

		m_aValue[3][0] =  mThis[3][0] * _mOther[0][0]; 
		m_aValue[3][0] += mThis[3][1] * _mOther[1][0];
		m_aValue[3][0] += mThis[3][2] * _mOther[2][0];
		m_aValue[3][0] += mThis[3][3] * _mOther[3][0];
			   	 			 
		m_aValue[3][1] =  mThis[3][0] * _mOther[0][1]; 
		m_aValue[3][1] += mThis[3][1] * _mOther[1][1];
		m_aValue[3][1] += mThis[3][2] * _mOther[2][1];
		m_aValue[3][1] += mThis[3][3] * _mOther[3][1];
			  		 	 	 
		m_aValue[3][2] =  mThis[3][0] * _mOther[0][2]; 
		m_aValue[3][2] += mThis[3][1] * _mOther[1][2];
		m_aValue[3][2] += mThis[3][2] * _mOther[2][2];
		m_aValue[3][2] += mThis[3][3] * _mOther[3][2];
			  		 	 	 
		m_aValue[3][3] =  mThis[3][0] * _mOther[0][3]; 
		m_aValue[3][3] += mThis[3][1] * _mOther[1][3];
		m_aValue[3][3] += mThis[3][2] * _mOther[2][3];
		m_aValue[3][3] += mThis[3][3] * _mOther[3][3];
	}
}}