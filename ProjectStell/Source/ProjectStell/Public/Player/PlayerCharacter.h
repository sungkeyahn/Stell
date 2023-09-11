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

//Ű�Է� ���ε� ��� ���� 
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
//�ִϸ��̼� ����
public:
	UPROPERTY()
		class UPlayerCharacterAnim* anim;
	class UPlayerCharacterAnim* GetCharacterAnim() {return anim;}
//��Ʈ�ѷ� ����
	class APlayerCharaterCtrl* PlayerCtrl;
//����,ī�޶� ����
private:
	UCameraComponent* camera;
	USpringArmComponent* springArm;
	FVector directionToMove = FVector::ZeroVector;
	void DefaultViewSetting();

//�ǰ� ����
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit)
		class USoundBase* HitSound;
	virtual float TakeDamage(float DamageAmout,struct FDamageEvent const& DamageEvent,class AController* EventInstigator, AActor* DamageCauser)override;

//���� ����
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
	UPROPERTY() //�ر��� ����
		TMap<int32, FItemInfoStruct> UnLockWeapons;

//������ ����
private:
	int32 SelectItemID = -1;
public:
	void SetSelectItemID(int32 i) { SelectItemID = i; }
	UFUNCTION(BlueprintCallable)
		void ItemContacted(class AItem* Item = nullptr);

//�޺� ����	
	UPROPERTY(VisibleAnywhere, Category = Combo)
		class UComboManager* Combo;
//���ݰ���
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UPlayerStat* Stat;
//hpȸ��
private:
	float CurHPRegenerationTime = 5.f; //��Ÿ�� ���� 
	FTimerHandle HPRegenerationTimerHandle;
	void HPRegeneration();
	float CurSPRegenerationTime = 5.f;
	FTimerHandle SPRegenerationTimerHandle;
	void SPRegeneration();

//�뽬 ����
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


//��� ����
private:
	void KillPlayer();
	FTimerHandle CharacterDstroyTimerHandle;
	float CharacterDstroyCoolTime = 0;
	void CharacterDestroyTimer();


//������ ����
public:
	// ISaveDataInclude��(��) ���� ��ӵ�
	virtual void DataSaveFun() override;
	virtual void DataLoadFun() override;
};
