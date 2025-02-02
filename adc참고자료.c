// 테이블에 대응하는 온도와 저항값의 쌍을 배열로 정의합니다.
// 이 배열은 실제 테이블에 기반하여 확장되어야 합니다.
const float temperatures[] = {
    -20,  -19,  -18,  -17,  -16,  -15,  -14,  -13,  -12,  -11,  -10,  -9,  -8,  -7,
    -6,  -5,  -4,  -3,  -2,  -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,  11,
    12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,
    28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,
    44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
    60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,
    76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,
    92,  93,  94,  95,  96,  97,  98,  99,  100
}; // 온도 샘플

const float resistances[] = {
    97839.6,  92302,  87112.4,  82247.1,  77683.7,  73401.8,  69382.3,  65607.7,  62061.6,
    58728.8,  55595.3,  52648,  49874.7,  47264.3,  44806.2,  42490.6,  40308.6,  38251.6,
    36311.7,  34481.7,  32754.7,  31124.3,  29584.7,  28130.1,  26755.6,  25456.2,  24227.4,
    23065,  21965,  20923.9,  19938,  190041,  18119.3,  17280.7,  16485.7,  15731.7,
    15016.4,  14337.6,  13693.3,  13081.6,  12500.5,  11948.5,  11423.9,  10925.2,  10451,
    10000,  9570.9,  9162.6,  8773.8,  8403.7,  8051.2,  7715.4,  7395.3,  7090.3,  6799.5,  6522.1,
    6257.6,  6005.1,  5764.2,  5534.2,  5314.6,  5104.9,  4904.5,  4713,  4530,  4355.1,  4187.8,
    4027.8,  3874.8,  3728.3,  3588.2,  3454,  3325.5,  3202.5,  3084.6,  2971.7,  2863.5,  2759.7,
    2660.3,  2564.9,  2473.4,  2385.6,  2301.4,  2220.6,  2143.1,  2068.6,  1997,  1928.3,  1862.3,
    1798.9,  1738,  1679.4,  1623.1,  1568.9,  1516.8,  1466.7,  1418.5,  1372.2,  1327.5,  1284.5,
    1243.1,  1203.3,  1164.9,  1127.9,  1092.3,  1058,  1024.9,  993,  962.3,  932.6,  904,
    876.4,  849.8,  824.1,  799.4,  775.4,  752.3,  730,  708.5,  687.7,  667.6
}; // 해당 온도에서의 저항값 샘플 

// 배열의 크기를 정의합니다. 실제 데이터의 양에 따라 조정해야 합니다.
const int tableSize = sizeof(temperatures) / sizeof(temperatures[0]);

float lookupTemperature(uint16_t adcValue) {
    float Vcc = 3.3; // 전원 전압
    float R1 = 10000; // 고정 저항값
    float Vo; // 서미스터 양단의 전압
    float R2; // 서미스터 저항값
    float temperature = 0.0;

    // ADC 값으로부터 전압 계산 (12비트 ADC의 경우)
    Vo = ((float)adcValue * Vcc) / 4095.0;

    // 서미스터의 저항값 계산 (서미스터와 고정 저항의 위치가 바뀐 경우)
    R2 = R1 / ((Vcc / Vo) - 1.0);

    // 서미스터의 저항값에 가장 가까운 테이블의 저항값을 찾고, 해당하는 온도를 반환합니다.
    // 이는 단순화된 예시이며, 실제 구현에서는 보간법을 사용할 수 있습니다.
    for (int i = 0; i < tableSize - 1; i++) {
        if (R2 <= resistances[i] && R2 >= resistances[i + 1]) {
            // 선형 보간을 사용하여 온도를 계산할 수 있습니다.
            float slope = (temperatures[i + 1] - temperatures[i]) / (resistances[i + 1] - resistances[i]);
            temperature = temperatures[i] + slope * (R2 - resistances[i]);
            break;
        }
    }
    return temperature;
}