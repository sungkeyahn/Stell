#pragma once

#include "ProjectStell.h"
#include "GameFramework/Character.h"

#include "Object/Item.h"
#include "Petten/SaveDataInclude.h"

#include "PlayerCharacter.generated.h"

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryChangedDelegate, FItemInfoStruct);

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
	class UPlayerCharacterAnim* GetCharacterAnim();
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
private:
	UPROPERTY() //해금한 무기들 데이터
		TArray<EWeaponType> UnLockWeapons;
	class AWeapon* leftWeapon;
	class AWeapon* rightWeapon;
	class AItem* ContactedItem = nullptr;

	UFUNCTION(BlueprintCallable)
		void PutOnWeapon(class AWeapon* newWeapon, int hand = 0);
public:
	class AWeapon* GetLeftWeapon();
	class AWeapon* GetRightWeapon();
	UFUNCTION(BlueprintCallable)
		void SetContactedItem(class AItem* Item=nullptr);


//콤보 관련	
	UPROPERTY(VisibleAnywhere, Category = Combo)
		class UComboManager* Combo;
//스텟관련
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UPlayerStat* Stat;
//hp회복
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HPRegen)
		float Regeneration = 1.f; //재생량 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HPRegen)
		float HPRegenerationTime = 5.f; //재생 쿨타임 
private:
	float CurRegenerationTime = 0.f; 
	FTimerHandle HPRegenerationTimerHandle;
	void HPRegeneration();

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


//아이템 관련
public:
	UFUNCTION(BlueprintCallable)
		AWeapon* ItemAcquisition(FItemInfoStruct info);

	// ISaveDataInclude을(를) 통해 상속됨
	virtual void DataSaveFun() override;
	virtual void DataLoadFun() override;
};
