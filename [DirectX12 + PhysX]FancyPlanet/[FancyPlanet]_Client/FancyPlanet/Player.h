#pragma once

#define DIR_FORWARD				0x01
#define DIR_BACKWARD			0x02
#define DIR_LEFT				0x04
#define DIR_RIGHT				0x08
#define DIR_UP					0x10
#define DIR_DOWN				0x20

#define FLAGE_NUM			3

#define JUMP 0
#define LAUNCH 1
#define ETC 2

#include "Object.h"
#include "Camera.h"

struct CB_PLAYER_INFO
{
	XMFLOAT4X4					m_xmf4x4World;
};

class CPlayer
	: public CGameObject,
	public PxUserControllerHitReport,
	public PxControllerBehaviorCallback
{
protected:
	XMFLOAT3					m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3					m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	XMFLOAT3					m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3     				m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	float           			m_fPitch = 0.0f;
	float           			m_fYaw = 0.0f;
	float           			m_fRoll = 0.0f;

	float           			m_fMaxVelocityXZ = 0.0f;
	float           			m_fMaxVelocityY = 0.0f;
	float           			m_fFriction = 0.0f;

	LPVOID						m_pPlayerUpdatedContext = NULL;
	LPVOID						m_pCameraUpdatedContext = NULL;

	CCamera						*m_pCamera = NULL;

	float						m_fSpeed = 1.0f;
	float						m_fTimeDelta = 0.0f;

	bool						m_bJumpState = false;
	//////////////////////////
	//		   PhysX		//
	//////////////////////////
	PxScene*					m_pScene;
private:
	XMFLOAT2				m_xmf2MousPos;
	bool						m_bFalgs[FLAGE_NUM];
	DWORD                m_dwStart[FLAGE_NUM];
	PxController * m_pPxCharacterController;

	CGameObject* pSphereMesh; // 충돌박스 입히기
	FLOAT m_fFallvelocity;
	FLOAT m_fFallAcceleration;
	CAnimationObject* m_pRenderObject;
	bool m_bAttackedState = false;
	bool m_bKeySwitch = true;
public:
	void SetMousPos(const float& fX, const float& fY)
	{
		m_xmf2MousPos.x = fX;
		m_xmf2MousPos.y = fY;
	}
	XMFLOAT2& GetMousPos()
	{
		return m_xmf2MousPos;
	}
	bool GetFlag(int index)
	{
		return m_bFalgs[index];
	}
	void SetFlag(int Index, bool bflag)
	{
		m_bFalgs[Index] = bflag;
	}
	DWORD& GetStartTime(int Index)
	{
		return m_dwStart[Index];
	}
	void SetStartTime(const DWORD& dwTime, int Index)
	{
		m_dwStart[Index] = dwTime;
	}
	bool GetJumpState()
	{
		return m_bJumpState;
	}
	void SetJumpState(bool bJumpState)
	{
		m_bJumpState = bJumpState;
	}
	void SetKeySwitch(bool bKeySwitch)
	{
		m_bKeySwitch = bKeySwitch;
	}
	bool GetKeySwitch()
	{
		return m_bKeySwitch;
	}
	void SetRenderObject(CAnimationObject* pRenderObject)
	{
		m_pRenderObject = pRenderObject;
	}
	CAnimationObject* GetRenderObject()
	{
		return m_pRenderObject;
	}
	XMFLOAT4X4& GetPlayerWorldTransform()
	{
		return m_pRenderObject->m_xmf4x4World;
	}
	bool GetBeAttackedState()
	{
		return m_bAttackedState;
	}
	void SetBeAttackedStateToFalse()
	{
		m_bAttackedState = false;
	}
	void AddForceAtLocalPos(PxRigidBody& body, const PxVec3& force, const PxVec3& pos, PxForceMode::Enum mode, bool wakeup = true);
	void AddForceAtPosInternal(PxRigidBody& body, const PxVec3& force, const PxVec3& pos, PxForceMode::Enum mode, bool wakeup);

	void onShapeHit(const PxControllerShapeHit & hit);
	virtual void onControllerHit(const PxControllersHit& hit) {}
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) {}

	void	BuildObject(PxPhysics* pPxPhysics, PxScene* pPxScene, PxMaterial *pPxMaterial, PxControllerManager *pPxControllerManager, void* pContext);

	// Implements PxControllerBehaviorCallback
	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxShape& shape, const PxActor& actor);
	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxController& controller);
	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxObstacle& obstacle);

	bool IsOnGround(void);

	void PxMove(float speed, float fTimeElapsed);

	//enum STATE_PLAYER { IDLE, MOVE, LYING, JUMP, DEAD };
	//DWORD			m_dwState;

	//void setDwState(DWORD dwstate) { m_dwState = dwstate; }
	//DWORD getDwState() { return m_dwState; }


public:
	CPlayer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature
		, void *pContext = NULL, int nMeshes = 1);
	virtual ~CPlayer();

	XMFLOAT3 GetPosition()
	{
		return(m_xmf3Position);
	}
	XMFLOAT3 GetLookVector()
	{
		return(m_xmf3Look);
	}
	XMFLOAT3 GetUpVector()
	{
		return(m_xmf3Up);
	}
	XMFLOAT3 GetRightVector()
	{
		return(m_xmf3Right);
	}

	float GetFallVelocity()
	{
		return(m_fFallvelocity);
	}
	void SetFallVelocity(float fVelocity) { m_fFallvelocity = fVelocity; }
	void SetFriction(float fFriction) { m_fFriction = fFriction; }
	void SetGravity(const XMFLOAT3& xmf3Gravity) { m_xmf3Gravity = xmf3Gravity; }
	void SetMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; }
	void SetMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; }
	void SetVelocity(const XMFLOAT3& xmf3Velocity) { m_xmf3Velocity = xmf3Velocity; }
	void SetPosition(const XMFLOAT3& xmf3Position) { Move(XMFLOAT3(xmf3Position.x - m_xmf3Position.x, xmf3Position.y - m_xmf3Position.y, xmf3Position.z - m_xmf3Position.z), false); }

	const XMFLOAT3& GetVelocity() const { return(m_xmf3Velocity); }
	float GetYaw() const { return(m_fYaw); }
	float GetPitch() const { return(m_fPitch); }
	float GetRoll() const { return(m_fRoll); }

	CCamera *GetCamera() { return(m_pCamera); }
	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }

	void Move(ULONG nDirection, float fDistance, bool bVelocity = false);
	void Move(const XMFLOAT3& xmf3Shift, bool bVelocity = false);
	void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);
	void Rotate(float x, float y, float z);

	void UpdateJumpState();
	void Update(float fTimeElapsed);

	virtual void OnPlayerUpdateCallback(float fTimeElapsed);
	void SetPlayerUpdatedContext(LPVOID pContext) { m_pPlayerUpdatedContext = pContext; }

	virtual void OnCameraUpdateCallback(float fTimeElapsed) { }
	void SetCameraUpdatedContext(LPVOID pContext) { m_pCameraUpdatedContext = pContext; }

	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void ChangeAnimation() {};
	virtual void UpdateTransform(XMFLOAT4X4 *pxmf4x4Parent = NULL) {};
	CCamera *OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode);

	virtual CCamera *ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed);

	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera = NULL);
	virtual void Animate(float fTimeElapsed);

protected:
	ID3D12Resource * m_pd3dcbPlayer = NULL;
	CB_PLAYER_INFO					*m_pcbMappedPlayer = NULL;
};

