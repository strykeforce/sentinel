# run with:
# jq -sf jq/quad_current.jq < sentinel.json | json2csv > quad_current.csv
#
[ .[] |
.results |
{
  QuadEncoder3vCurrent: .QuadEncoderLogicALow3vCurrent.actual,
  QuadEncoder5vCurrent: .QuadEncoderLogicALow5vCurrent.actual
} ] +
[ .[] |
.results |
{
  QuadEncoder3vCurrent: .QuadEncoderLogicBLow3vCurrent.actual,
  QuadEncoder5vCurrent: .QuadEncoderLogicBLow5vCurrent.actual
} ] +
[ .[] |
.results |
{
  QuadEncoder3vCurrent: .QuadEncoderLogicILow3vCurrent.actual,
  QuadEncoder5vCurrent: .QuadEncoderLogicILow5vCurrent.actual
} ]
