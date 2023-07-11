#pragma once

#include "ProjectStell.h"
#include "GameFramework/Character.h"
#include "Object/Item.h"
#include "PlayerCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryChangedDelegate, FItemInfoStruct);

UCLASS()
class PROJECTSTELL_API APlayerCharacter : public ACharacter
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
//�ִϸ��̼� ����
public:
	UPROPERTY()
		class UPlayerCharacterAnim* anim;
	class UPlayerCharacterAnim* GetCharacterAnim();
//��Ʈ�ѷ� ����
	class APlayerCharaterCtrl* PlayerCtrl;
//����,ī�޶� ����
public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* springArm;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* camera;
protected:
	FVector directionToMove = FVector::ZeroVector;
	FRotator armRotationTo = FRotator::ZeroRotator;
	float armLengthTo = 0.0f;
	float armLengthSpeed = 3.0f;
	float armRotationSpeed = 10.0f;
	void DefaultViewSetting();
//�ǰ� ����
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit)
		class USoundBase* HitSound;
	virtual float TakeDamage(float DamageAmout,struct FDamageEvent const& DamageEvent,class AController* EventInstigator, AActor* DamageCauser)override;

//���� ���� 
private:
	class AWeapon* leftWeapon;
	class AWeapon* rightWeapon;
	class AItem* ContactedItem = nullptr;

	UFUNCTION(BlueprintCallable)
		void PutOnWeapon(class AWeapon* newWeapon, int hand = 0);
public:
	void Equipment_Left();
	void Equipment_Right();
	UFUNCTION(BlueprintCallable)
		void SetContactedItem(class AItem* Item=nullptr);
	class AWeapon* GetLeftWeapon();
	class AWeapon* GetRightWeapon();

//�޺� ����	
	UPROPERTY(VisibleAnywhere, Category = Combo)
		class UComboManager* Combo;
//���ݰ���
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UPlayerStat* Stat;
//hpȸ����� 
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HPRegen)
		float Regeneration = 1.f; //����� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HPRegen)
		float HPRegenerationTime = 5.f; //��� ��Ÿ�� 
private:
	float CurRegenerationTime = 0.f; 
	FTimerHandle HPRegenerationTimerHandle;
	void HPRegeneration();

//�뽬 ����
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* DashMontage = nullptr;
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
private:
	TMap<int32,FItemInfoStruct>Inventory;
	void LoadInvenData(); //����� ������ �κ� ������ �ҷ��� �κ��� �ʱ�ȭ�ϴ� �Լ�
	bool AddItem(FItemInfoStruct info); //�κ��� �������� �߰��ϴ� �Լ� -> �ߺ� ������ �˻�, ������ �����ǵ��� 
public:
	FOnInventoryChangedDelegate OnInventoryChanged;
	FItemInfoStruct* GetItem(int32 ID); //�κ��� �ش� �������� �ִ��� üũ�� �����Ϳ� ����
	TMap<int32, FItemInfoStruct> GetInventory();
	UFUNCTION(BlueprintCallable)
		AWeapon* ItemAcquisition(FItemInfoStruct info); //�ΰ��ӿ��� ������ ���Ϳ� ����� �� ȣ��� �Լ� ,���� �κ� ���¸� üũ�ؼ� ������ ������ �߰�
	//�κ��� ���� ���氪�� �����쿡 ���� ��������Ʈ �ʿ��ҵ�?
};
