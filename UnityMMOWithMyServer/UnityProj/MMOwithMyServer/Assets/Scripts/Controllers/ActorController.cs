using Google.Protobuf.Protocol;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static Define;

public class ActorController : BaseController
{
    HpBar _hpBar;
    public override StatInfo Stat
    {
        get { return base.Stat; }
        set
        {
            base.Stat = value;
            UpdateHpBar();
        }
    }

    public override int Hp
    {
        get { return base.Hp; }
        set
        {
            base.Hp = value;
            UpdateHpBar();
        }
    }

    protected override void UpdateIdle()
    {

    }

    protected override void UpdateSkill()
    {

    }

    protected override void UpdateDead()
    {

    }

    public virtual void UseSkill(int skillId)
    {

    }

    protected void AddHpBar()
    {
        GameObject obj = Managers.Resource.Instantiate("UI/HpBar", transform);
        obj.transform.localPosition = new Vector3(0, 0.5f, 0); // Hp 바 위치가 Actor 머리 위가 되도록 임시 설정
        obj.name = "HpBar";
        _hpBar = obj.GetComponent<HpBar>();
        UpdateHpBar();
    }

    void UpdateHpBar()
    {
        if(_hpBar == null)
        {
            return;
        }

        float ratio = 0.0f;
        if(Stat.MaxHp > 0)
        {
            ratio = ((float)Hp) / Stat.MaxHp;
        }

        _hpBar.SetHpBar(ratio);
    }

    protected override void Init()
    {
        base.Init();
        AddHpBar();
    }

    public virtual void OnDamaged()
    {

    }

    public virtual void OnDead()
    {
        State = ActorState.Dead;

        GameObject effect = Managers.Resource.Instantiate("Effect/DeathEffect");
        effect.transform.position = transform.position;
        effect.GetComponent<Animator>().Play("Death");
        GameObject.Destroy(effect, 0.5f);
    }
}
