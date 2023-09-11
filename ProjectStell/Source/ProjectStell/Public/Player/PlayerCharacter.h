#pragma once

#include "ProjectStell.h"
#include "GameFramework/Character.h"

#include "Object/Item.h"
#include "Petten/SaveDataInclude.h"

#include "PlayerCharacter.generated.h"

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryChangedDelegate, FItemInfoStruct);
DECLARE_MULTICAST_DELEGATE(FOnHaveWeaponChanged);

UCLASS()
class PROJECTSTELL_API APlayerCharacter : public ACharacter ,public ISaveDataInclude
{
	GENERATED_BODY()
public:
	APlayerCharacter();
protected:
	virtual void BeginPlay() override;
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents()override;

//키입력 바인딩 기능 관련 
public:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void LeftAttack();
	void RightAttack();
	void Evasion();
	void Equipment_Left();
	void Equipment_Right();
//애니메이션 관련
public:
	UPROPERTY()
		class UPlayerCharacterAnim* anim;
	class UPlayerCharacterAnim* GetCharacterAnim() {return anim;}
//컨트롤러 관련
	class APlayerCharaterCtrl* PlayerCtrl;
//시점,카메라 관련
private:
	UCameraComponent* camera;
	USpringArmComponent* springArm;
	FVector directionToMove = FVector::ZeroVector;
	void DefaultViewSetting();

//피격 관련
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit)
		class USoundBase* HitSound;
	virtual float TakeDamage(float DamageAmout,struct FDamageEvent const& DamageEvent,class AController* EventInstigator, AActor* DamageCauser)override;

//무기 관련
public:
	FOnHaveWeaponChanged OnHaveWeaponChanged;
private:
	class AWeapon* leftWeapon;
	class AWeapon* rightWeapon;
	AWeapon* SpawnWeapon(FItemInfoStruct info);
	AWeapon* SpawnWeapon(FItemInfoStruct* info);
	void EquipWeapon(class AWeapon* newWeapon, int hand = 0);
public:
	class AWeapon* GetLeftWeapon();
	class AWeapon* GetRightWeapon();
	UPROPERTY() //해금한 무기
		TMap<int32, FItemInfoStruct> UnLockWeapons;

//아이템 관련
private:
	int32 SelectItemID = -1;
public:
	void SetSelectItemID(int32 i) { SelectItemID = i; }
	UFUNCTION(BlueprintCallable)
		void ItemContacted(class AItem* Item = nullptr);

//콤보 관련	
	UPROPERTY(VisibleAnywhere, Category = Combo)
		class UComboManager* Combo;
//스텟관련
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UPlayerStat* Stat;
//hp회복
private:
	float CurHPRegenerationTime = 5.f; //쿨타임 변수 
	FTimerHandle HPRegenerationTimerHandle;
	void HPRegeneration();
	float CurSPRegenerationTime = 5.f;
	FTimerHandle SPRegenerationTimerHandle;
	void SPRegeneration();

//대쉬 관련
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* DashMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* DashAttackMontage = nullptr;
	UPROPERTY(VisibleAnywhere,Category = Dash)
		bool IsDashing;
	UPROPERTY(VisibleAnywhere, Category = Dash)
		int32 DashCount = 2;
	UPROPERTY(VisibleAnywhere,Category = Dash)
		int32 DashCoolTime = 10;
	FTimerHandle DashCoolTimerHandle;
	void DashCoolTimer();


//사망 관련
private:
	void KillPlayer();
	FTimerHandle CharacterDstroyTimerHandle;
	float CharacterDstroyCoolTime = 0;
	void CharacterDestroyTimer();


//데이터 관련
public:
	// ISaveDataInclude을(를) 통해 상속됨
	virtual void DataSaveFun() override;
	virtual void DataLoadFun() override;
};
