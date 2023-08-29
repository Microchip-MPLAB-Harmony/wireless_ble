<#if GAP_DSADV_EN>
            bool flag=false;

            flag = APP_BleDsadvIsEnable();

            if (flag == false)
            {
                APP_BleStackInit();
                APP_BleDsadvStart(flag);
    <#if BLE_SYS_SLEEP_MODE_EN>
                if (!(RTC_REGS->MODE0.RTC_CTRLA & RTC_MODE0_CTRLA_ENABLE_Msk))
                {
                    RTC_Timer32Start();
                }
    </#if>
            }
            else   //Wake up from deep sleep by RTC/INT0/Watch dog
            {
                 APP_BleDsadvStart(flag);
            }
<#else>
            APP_BleStackInit();
    <#if BLE_SYS_SLEEP_MODE_EN>
            if (!(RTC_REGS->MODE0.RTC_CTRLA & RTC_MODE0_CTRLA_ENABLE_Msk))
            {
                RTC_Timer32Start();
            }
    </#if>
</#if>
