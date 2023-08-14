#pragma once

#include "ProjectStell.h"
#include "GameFramework/Character.h"

#include "NPC/EnemyAnim.h"

#include "Enemy.generated.h"


class EnemyState
{
public:
	EnemyState(EEnemyState _curState, AEnemy* _actor) : curState(_curState), actor(_actor){};
	virtual ~EnemyState() {};
	void SetState(EEnemyState newState);
	EEnemyState GetState() {return curState;}
private:
	EEnemyState curState;
	AEnemy* actor;
};

DECLARE_MULTICAST_DELEGATE(FOnEnemyDeadDelegate);

UCLASS()
class PROJECTSTELL_API AEnemy : public ACharacter
{
	GENERATED_BODY()
//�ʱ�ȭ
public:
	AEnemy();
protected:
	virtual void BeginPlay() override;
public:
	virtual void PostInitializeComponents()override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public :
	int32 Num=-1; //���� �ĺ��� 
	void SetEnemyIndex(int32 i) { Num = i;}
//��Ʈ�ѷ� ����
protected:
	UPROPERTY()
		class AEnemyCtrl* ctrl;
public:
	AEnemyCtrl* GetCtrl() { return ctrl; }
	void StopUnit();
	void RunUnit();


//���� ����
protected:
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UStat* stat;
//�ִϸ��̼� ����
protected:
	UPROPERTY()
		class UEnemyAnim* anim;
public:
	 UEnemyAnim* GetAnim() { return anim; }
//���ӻ���, ���� ���°���
protected: 
	EnemyState* state;
public:
	EEnemyState GetState() { return state->GetState(); }

	float senseRange = 500.f;
	float attackSenseRange = 100.f;

	UPROPERTY(EditAnywhere, Category = Behavior)
		class UAttack* atk;
	UPROPERTY(EditAnywhere, Category = Behavior)
		class UHit* hit;

//��� ����
public:
	FOnEnemyDeadDelegate OnDead;
	void Dead();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		float DeadTime = 0;
	FTimerHandle DeadTimerHandle;
};
