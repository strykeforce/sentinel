# run with:
# jq -sf jq/limit_current.jq < sentinel.json | json2csv > limit_current.csv
#
[ .[] |
.results |
{
  LimitSwitch3vCurrent: .LimitSwitchFwdClosed3vCurrent.actual,
  LimitSwitch5vCurrent: .LimitSwitchFwdClosed5vCurrent.actual
} ] +
[ .[] |
.results |
{
  LimitSwitch3vCurrent: .LimitSwitchRevClosed3vCurrent.actual,
  LimitSwitch5vCurrent: .LimitSwitchRevClosed5vCurrent.actual
} ]
