#pragma once

#include "ProjectStell.h"

class AActor;

class  Command
{
public:
	Command();
	virtual ~Command();
	virtual void Execute() = 0;
	//virtual void Execute(AActor* actor) = 0;
	virtual void Cancel() = 0; //��ɿ� ���� ���� ���

};
class MoveCommand : public Command
{
public:
	MoveCommand(AActor* actor, float newAxisValue,bool vectorIsForward) : _actor(actor), _newAxisValue(newAxisValue),_vectorIsForward(vectorIsForward) {}
	// Command��(��) ���� ��ӵ�
	virtual void Execute() final;
	virtual void Cancel() final;
private:
	AActor* _actor;
	float _newAxisValue;
	bool _vectorIsForward;
};
class AttackCommand : public Command
{
public:
	AttackCommand(AActor* actor, bool isLeftAttack) : _actor(actor), _isLeftAttack(isLeftAttack) {}
	// Command��(��) ���� ��ӵ�
	virtual void Execute() final;
	virtual void Cancel() final;
private:
	AActor* _actor;
	bool _isLeftAttack;
	//����� �޺� �־ �����Ҽ� �ֵ��� �����ϱ� 
};
class EvasionCommand : public Command
{
public:
	EvasionCommand(AActor* actor) : _actor(actor){}
	// Command��(��) ���� ��ӵ�
	virtual void Execute() final;
	virtual void Cancel() final;
private:
	AActor* _actor;
};
class EquipmentCommand : public Command
{
public:
	EquipmentCommand(AActor* actor, bool isLeftEquipment) : _actor(actor), _isLeftEquipment(isLeftEquipment){}
	// Command��(��) ���� ��ӵ�
	virtual void Execute() final;
	virtual void Cancel() final;
private:
	AActor* _actor;
	bool _isLeftEquipment;
};
class ShowGUICommand : public Command
{
public:
	ShowGUICommand(AActor* actor, int32 uiNumber) : _actor(actor), _uiNumber(uiNumber) {}
	// Command��(��) ���� ��ӵ�
	virtual void Execute() final;
	virtual void Cancel() final;
private:
	AActor* _actor;
	int32 _uiNumber;
};

