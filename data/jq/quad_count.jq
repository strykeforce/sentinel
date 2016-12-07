# run with:
# jq -sf jq/quad_count.jq < sentinel.json | json2csv > quad_count.csv
#
[ .[] |
.results |
{
  QuadEncoderCountUUT: .QuadEncoderCount.actual,
  QuadEncoderCountRef: .QuadEncoderCount.expected
} ]
