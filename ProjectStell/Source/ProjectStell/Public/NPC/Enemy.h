#pragma once

#include "ProjectStell.h"
#include "GameFramework/Character.h"

#include "Petten/SpawnAble.h"

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
class PROJECTSTELL_API AEnemy : public ACharacter, public ISpawnAble
{
	GENERATED_BODY()
//초기화
public:
	AEnemy();
protected:
	virtual void BeginPlay() override;
public:
	virtual void PostInitializeComponents()override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
//컨트롤러 관련
protected:
	UPROPERTY()
		class AEnemyCtrl* ctrl;
public:
	AEnemyCtrl* GetCtrl() { return ctrl; }
private:
	UPROPERTY()
		float curUnitStopTime = 0.f;
	FTimerHandle UnitStopTimerHandle;
public:
	void StopUnit();
	void StopUnit(float stopTime);
	void RunUnit();


//스텟 관련
protected:
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UStat* stat;
//애니메이션 관련
protected:
	UPROPERTY()
		class UEnemyAnim* anim;
public:
	 UEnemyAnim* GetAnim() { return anim; }
//게임상태, 몬스터 상태관련
protected: 
	EnemyState* state;
public:
	void SetState(EEnemyState newState) { state->SetState(newState); }
	EEnemyState GetState() { return state->GetState(); }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range, Meta = (AllowPrivateAccess = true))
		float senseRange = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range, Meta = (AllowPrivateAccess = true))
		float attackSenseRange = 100.f;

	UPROPERTY(EditAnywhere, Category = Behavior)
		class UAttack* atk;
	UPROPERTY(EditAnywhere, Category = Behavior)
		class UHit* hit;

//사망 관련
public:
	FOnEnemyDeadDelegate OnDead;
	void Dead();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		float DeadTime = 0;
	FTimerHandle DeadTimerHandle;
};
