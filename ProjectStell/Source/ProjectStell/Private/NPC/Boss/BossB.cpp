// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Boss/BossB.h"
#include "NPC/Boss/BossBCtrl.h"
#include "NPC/EnemyAnim.h"

ABossB::ABossB()
{
	SetState(EEnemyState::SuperArmor);
}
void ABossB::BeginPlay()
{
	Super::BeginPlay();
	anim->OnMontageEnded.AddDynamic(this, &ABossB::OnSkillEndFun);
	GetWorldTimerManager().SetTimer(SkillCoolTimerHandle, this, &ABossB::SkillCoolTimer, 1.0f, true);
}
void ABossB::SkillCoolTimer()
{
	CurCoolTime++;
	if (CurCoolTime > SkillCoolTime)
	{
		CurCoolTime = 0.f;
		Cast<ABossBCtrl>(GetController())->SkillActivate(true);
	}
}
void ABossB::JumpAttack()
{
	anim->PlayEnemyMontage(SkillAttackInfo.montage);
	atk->Attack(SkillAttackInfo);
	//StopUnit(1.f);
}
void ABossB::OnSkillEndFun(UAnimMontage* Montage, bool bInterrupted)
{
	OnSkillEnd.Broadcast();
}

