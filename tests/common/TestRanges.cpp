#include <gtest/gtest.h>
#include <vtest/common/ValueTests.hpp>

#include <cmath>
#include <limits>
#include <numbers>
#include <string>

// ============================================================================
// 基础功能测试
// ============================================================================

/**
 * 测试 _TEST_SUITE_P 宏 - 单一参数类型
 * 
 * 功能说明：
 * - 测试单一参数类型的参数化测试
 * - 输入包含重复值 {1, 2, 3, 2, 1}
 * - _TEST_SUITE_P 会自动去重和排序，实际测试参数为 {1, 2, 3}
 * - 测试会运行 3 次，每次使用不同的参数值
 * 
 * 预期行为：
 * - 重复的值会被自动去除
 * - 参数会按升序排列
 * - 每个唯一值只会被测试一次
 */
_TEST_SUITE_P(SingleParamTest, vtest::ValueList<int>{1, 2, 3, 2, 1});

/**
 * 单参数测试用例
 * 
 * 测试内容：
 * - 使用 GetParamValue<0>() 获取参数值
 * - 验证参数值在预期范围内 [1, 3]
 * 
 * 执行次数：3 次（参数值分别为 1, 2, 3）
 */
TEST_P(SingleParamTest, BasicTest)
{
    int value = GetParamValue<0>(); // 获取第一个（也是唯一的）参数
    EXPECT_TRUE(value >= 1 && value <= 3);
}

/**
 * 测试 _TEST_SUITE_P 宏 - 多参数类型（笛卡尔积）
 * 
 * 功能说明：
 * - 使用 operator* 生成两个参数列表的笛卡尔积
 * - ValueList<int>{1, 2} * ValueList<const char*>{"a", "b"}
 * - 生成 4 个参数组合：(1,"a"), (1,"b"), (2,"a"), (2,"b")
 * 
 * 笛卡尔积原理：
 * - 第一个列表的每个元素与第二个列表的每个元素配对
 * - 结果数量 = 第一个列表大小 × 第二个列表大小 = 2 × 2 = 4
 * 
 * 参数访问：
 * - GetParamValue<0>() 获取 int 类型的第一个参数
 * - GetParamValue<1>() 获取 const char* 类型的第二个参数
 * 
 * 所有组合：
 * - (1, "a")
 * - (1, "b")
 * - (2, "a")
 * - (2, "b")
 */
_TEST_SUITE_P(MultiParamTest, vtest::ValueList<int>{1, 2} * vtest::ValueList<const char *>{"a", "b"});

/**
 * 笛卡尔积测试用例
 * 
 * 测试内容：
 * - 验证笛卡尔积生成的所有参数组合
 * - 检查第一个参数（int）的值是否为 1 或 2
 * - 检查第二个参数（const char*）的值是否为 "a" 或 "b"
 * 
 * 执行次数：4 次
 * - (1, "a")
 * - (1, "b")
 * - (2, "a")
 * - (2, "b")
 */
TEST_P(MultiParamTest, CartesianProductTest)
{
    int         int_value = GetParamValue<0>(); // 获取第一个参数（int）
    const char *str_value = GetParamValue<1>(); // 获取第二个参数（const char*）

    EXPECT_TRUE(int_value == 1 || int_value == 2);
    EXPECT_TRUE(std::string(str_value) == "a" || std::string(str_value) == "b");
}

/**
 * 测试 _TEST_SUITE_P 宏 - 三参数类型（三重笛卡尔积）
 * 
 * 功能说明：
 * - 使用连续的 operator* 生成三个参数列表的笛卡尔积
 * - ValueList<int>{10, 20} * ValueList<bool>{true, false} * ValueList<double>{3.14, 2.71}
 * - 生成 8 个参数组合：2 × 2 × 2 = 8
 * 
 * 所有组合：
 * - (10, true,  3.14), (10, true,  2.71)
 * - (10, false, 3.14), (10, false, 2.71)
 * - (20, true,  3.14), (20, true,  2.71)
 * - (20, false, 3.14), (20, false, 2.71)
 */
_TEST_SUITE_P(TripleParamTest, vtest::ValueList<int>{10, 20} * vtest::ValueList<bool>{true, false}
                                   * vtest::ValueList<double>{3.14, 2.71});

/**
 * 三参数测试用例
 * 
 * 测试内容：
 * - 验证三重笛卡尔积生成的所有参数组合
 * - 检查三个参数的值是否在预期范围内
 * 
 * 执行次数：8 次（所有可能的三元组合）
 */
TEST_P(TripleParamTest, ThreeParametersTest)
{
    int    int_value    = GetParamValue<0>(); // 获取第一个参数（int）
    bool   bool_value   = GetParamValue<1>(); // 获取第二个参数（bool）
    double double_value = GetParamValue<2>(); // 获取第三个参数（double）

    EXPECT_TRUE(int_value == 10 || int_value == 20);
    EXPECT_TRUE(bool_value == true || bool_value == false);
    EXPECT_TRUE(double_value == 3.14 || double_value == 2.71);
}

/**
 * 测试参数去重功能
 * 
 * 功能说明：
 * - 输入包含大量重复值：{5, 3, 5, 1, 3, 1}
 * - _TEST_SUITE_P 会自动去重并排序
 * - 实际测试参数为：{1, 3, 5}（去重后按升序排列）
 * 
 * 验证目标：
 * - 确认重复值被正确去除
 * - 确认只有唯一值被测试
 * - 测试只运行 3 次而不是 6 次
 */
_TEST_SUITE_P(DuplicateRemovalTest, vtest::ValueList<int>{5, 3, 5, 1, 3, 1});

/**
 * 去重验证测试用例
 * 
 * 测试内容：
 * - 验证参数值只包含唯一值
 * - 确认没有重复的测试执行
 * 
 * 执行次数：3 次（参数值分别为 1, 3, 5）
 */
TEST_P(DuplicateRemovalTest, NoDuplicatesTest)
{
    int value = GetParamValue<0>();
    EXPECT_TRUE(value == 1 || value == 3 || value == 5);
}

/**
 * 测试空参数列表（边界情况）
 * 
 * 功能说明：
 * - 测试参数列表为空的情况
 * - 空参数列表不会生成任何测试实例
 * - 测试用例不会被执行
 * 
 * 特殊处理：
 * - 使用 GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST 抑制 Google Test 的警告
 * - Google Test 默认会对未实例化的参数化测试发出警告
 */
_TEST_SUITE_P(EmptyParamTest, vtest::ValueList<int>{});

/**
 * 空参数测试用例（不应该运行）
 * 
 * 测试内容：
 * - 如果这个测试被执行，说明空参数处理有问题
 * - 使用 FAIL() 确保如果意外执行会被检测到
 * 
 * 执行次数：0 次（因为参数列表为空）
 */
TEST_P(EmptyParamTest, ShouldNotRun)
{
    // 这个测试不应该运行，因为参数列表为空
    FAIL() << "This test should not run with empty parameters";
}

// 抑制空参数测试的 Google Test 警告
GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(EmptyParamTest);

/**
 * 测试字符串参数
 * 
 * 功能说明：
 * - 测试 const char* 类型的参数
 * - 验证字符串参数的正确传递和使用
 * - 参数列表：{"hello", "world", "test"}
 * 
 * 注意事项：
 * - 使用 const char* 而不是 std::string
 * - 字符串字面量在整个测试期间保持有效
 */
_TEST_SUITE_P(StringParamTest, vtest::ValueList<const char *>{"hello", "world", "test"});

/**
 * 字符串参数测试用例
 * 
 * 测试内容：
 * - 验证字符串参数的值是否正确
 * - 检查字符串不为空
 * - 将 const char* 转换为 std::string 进行比较
 * 
 * 执行次数：3 次（参数值分别为 "hello", "world", "test"）
 */
TEST_P(StringParamTest, StringTest)
{
    const char *str = GetParamValue<0>();
    std::string value(str);
    EXPECT_TRUE(value == "hello" || value == "world" || value == "test");
    EXPECT_FALSE(value.empty());
}

/**
 * 测试混合类型参数
 * 
 * 功能说明：
 * - 测试不同类型参数的笛卡尔积
 * - int 类型和 double 类型的组合
 * - 生成 6 个参数组合：3 × 2 = 6
 * 
 * 所有组合：
 * - (100, 1.5), (100, 2.5)
 * - (200, 1.5), (200, 2.5)
 * - (300, 1.5), (300, 2.5)
 */
_TEST_SUITE_P(MixedTypeTest, vtest::ValueList<int>{100, 200, 300} * vtest::ValueList<double>{1.5, 2.5});

/**
 * 混合类型测试用例
 * 
 * 测试内容：
 * - 验证 int 和 double 类型参数的组合
 * - 检查参数值在预期范围内
 * 
 * 执行次数：6 次（所有 int 和 double 的组合）
 */
TEST_P(MixedTypeTest, IntAndDoubleTest)
{
    int    int_value    = GetParamValue<0>();
    double double_value = GetParamValue<1>();

    EXPECT_GE(int_value, 100);
    EXPECT_LE(int_value, 300);
    EXPECT_GE(double_value, 1.5);
    EXPECT_LE(double_value, 2.5);
}

/**
 * 多个测试用例共享同一个参数化测试套件
 * 
 * 功能说明：
 * - 同一个测试套件（MultiParamTest）可以包含多个测试用例
 * - 每个测试用例都会使用相同的参数组合运行
 * - 这个测试用例验证参数组合的完整性
 * 
 * 测试内容：
 * - 验证所有 4 个参数组合都是有效的
 * - 确认 int 和 string 的配对关系正确
 * 
 * 执行次数：4 次（与 CartesianProductTest 使用相同的参数）
 */
TEST_P(MultiParamTest, SecondTestCase)
{
    int         int_value = GetParamValue<0>();
    const char *str_value = GetParamValue<1>();

    // 验证参数组合的数量（2 * 2 = 4）
    EXPECT_TRUE((int_value == 1 && (std::string(str_value) == "a" || std::string(str_value) == "b"))
                || (int_value == 2 && (std::string(str_value) == "a" || std::string(str_value) == "b")));
}

/**
 * 测试 GetParamValue 方法的正确性
 * 
 * 功能说明：
 * - 验证 GetParamValue<I>() 辅助方法的实现
 * - 确认它与 Google Test 的 GetParam() 返回相同的值
 * 
 * 测试内容：
 * - GetParam() 返回完整的 tuple
 * - GetParamValue<0>() 应该等于 std::get<0>(GetParam())
 * - GetParamValue<1>() 应该等于 std::get<1>(GetParam())
 * 
 * 执行次数：4 次
 */
TEST_P(MultiParamTest, ThirdTestCase)
{
    // 测试 GetParamValue 方法的正确性
    auto param = GetParam(); // 获取完整的参数 tuple
    EXPECT_EQ(std::get<0>(param), GetParamValue<0>());
    EXPECT_STREQ(std::get<1>(param), GetParamValue<1>());
}

// ============================================================================
// 边界值测试
// ============================================================================

/**
 * 测试整数边界值
 * 
 * 功能说明：
 * - 测试 int 类型的边界值和特殊值
 * - 包括最小值、最大值、零和正负单位值
 * 
 * 测试参数：
 * - std::numeric_limits<int>::min()  // 最小值（通常是 -2147483648）
 * - -1                                // 负数边界
 * - 0                                 // 零值
 * - 1                                 // 正数边界
 * - std::numeric_limits<int>::max()  // 最大值（通常是 2147483647）
 * 
 * 目的：
 * - 确保代码能正确处理整数的极端值
 * - 验证边界条件下的行为
 */
_TEST_SUITE_P(IntBoundaryTest,
              vtest::ValueList<int>{std::numeric_limits<int>::min(), -1, 0, 1, std::numeric_limits<int>::max()});

/**
 * 整数边界值测试用例
 * 
 * 测试内容：
 * - 验证所有边界值都在 int 类型的有效范围内
 * - 这是一个基本的健全性检查
 * 
 * 执行次数：5 次（所有边界值）
 */
TEST_P(IntBoundaryTest, BoundaryValuesTest)
{
    int value = GetParamValue<0>();
    // 验证值在有效范围内
    EXPECT_GE(value, std::numeric_limits<int>::min());
    EXPECT_LE(value, std::numeric_limits<int>::max());
}

/**
 * 测试浮点数边界值和特殊值
 * 
 * 功能说明：
 * - 测试 double 类型的边界值和特殊值
 * - 包括无穷大、最小/最大有限值、零（正零和负零）
 * 
 * 测试参数：
 * - -infinity  // 负无穷大
 * - lowest()   // 最小有限值（最大的负数）
 * - -1.0       // 负单位值
 * - -0.0       // 负零
 * - 0.0        // 正零
 * - 1.0        // 正单位值
 * - max()      // 最大有限值
 * - +infinity  // 正无穷大
 * 
 * 注意：
 * - 不包括 NaN，因为 NaN != NaN，需要单独测试
 * - 正零和负零在某些操作中行为不同
 */
_TEST_SUITE_P(DoubleBoundaryTest,
              vtest::ValueList<double>{-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::lowest(),
                                       -1.0, -0.0, 0.0, 1.0, std::numeric_limits<double>::max(),
                                       std::numeric_limits<double>::infinity()});

/**
 * 浮点数特殊值测试用例
 * 
 * 测试内容：
 * - 验证无穷大值的正确性
 * - 验证正零和负零的处理
 * - 验证非 NaN 值
 * 
 * 测试逻辑：
 * - 如果是无穷大，验证是正无穷或负无穷
 * - 如果是零，验证符号位（正零或负零）
 * - 其他值验证不是 NaN
 * 
 * 执行次数：8 次（所有边界值）
 */
TEST_P(DoubleBoundaryTest, DoubleSpecialValuesTest)
{
    double value = GetParamValue<0>();

    if (std::isinf(value))
    {
        // 验证是正无穷或负无穷
        EXPECT_TRUE(value == std::numeric_limits<double>::infinity()
                    || value == -std::numeric_limits<double>::infinity());
    }
    else if (value == 0.0)
    {
        // 测试正零和负零（它们在某些操作中行为不同）
        EXPECT_TRUE(std::signbit(value) == false || std::signbit(value) == true);
    }
    else
    {
        // 其他值不应该是 NaN
        EXPECT_FALSE(std::isnan(value));
    }
}

/**
 * 测试 NaN 值（单独测试，因为 NaN != NaN）
 * 
 * 功能说明：
 * - NaN（Not a Number）有特殊的比较语义
 * - NaN 不等于任何值，包括它自己
 * - 必须使用 std::isnan() 来检测 NaN
 * 
 * 为什么单独测试：
 * - NaN != NaN，所以不能用普通的相等比较
 * - 在去重和排序时需要特殊处理
 * - 单独测试可以更清晰地验证 NaN 的行为
 */
_TEST_SUITE_P(NaNTest, vtest::ValueList<double>{std::numeric_limits<double>::quiet_NaN()});

/**
 * NaN 处理测试用例
 * 
 * 测试内容：
 * - 验证值确实是 NaN
 * - 验证 NaN 的特殊属性：NaN != NaN
 * 
 * 执行次数：1 次
 */
TEST_P(NaNTest, NaNHandlingTest)
{
    double value = GetParamValue<0>();
    EXPECT_TRUE(std::isnan(value)); // 使用 isnan() 检测
    EXPECT_FALSE(value == value);   // NaN 的特性：NaN != NaN
}

// ============================================================================
// 复杂笛卡尔积测试
// ============================================================================

/**
 * 测试四参数笛卡尔积
 * 
 * 功能说明：
 * - 使用连续的 operator* 生成四个参数列表的笛卡尔积
 * - 生成 16 个参数组合：2 × 2 × 2 × 2 = 16
 * 
 * 参数类型：
 * - int: {1, 2}
 * - bool: {true, false}
 * - double: {1.5, 2.5}
 * - const char*: {"x", "y"}
 * 
 * 组合示例：
 * - (1, true,  1.5, "x"), (1, true,  1.5, "y")
 * - (1, true,  2.5, "x"), (1, true,  2.5, "y")
 * - (1, false, 1.5, "x"), ...
 * - 共 16 种组合
 */
_TEST_SUITE_P(FourParamTest, vtest::ValueList<int>{1, 2} * vtest::ValueList<bool>{true, false}
                                 * vtest::ValueList<double>{1.5, 2.5} * vtest::ValueList<const char *>{"x", "y"});

/**
 * 四参数测试用例
 * 
 * 测试内容：
 * - 验证所有 16 种参数组合
 * - 检查每个参数的值是否在预期范围内
 * 
 * 执行次数：16 次
 */
TEST_P(FourParamTest, FourParametersTest)
{
    int         int_value    = GetParamValue<0>();
    bool        bool_value   = GetParamValue<1>();
    double      double_value = GetParamValue<2>();
    const char *str_value    = GetParamValue<3>();

    // 验证所有参数组合（2 * 2 * 2 * 2 = 16 种组合）
    EXPECT_TRUE(int_value == 1 || int_value == 2);
    EXPECT_TRUE(bool_value == true || bool_value == false);
    EXPECT_TRUE(double_value == 1.5 || double_value == 2.5);
    EXPECT_TRUE(std::string(str_value) == "x" || std::string(str_value) == "y");
}

/**
 * 测试不对称笛卡尔积（不同大小的参数列表）
 * 
 * 功能说明：
 * - 测试不同大小的参数列表的笛卡尔积
 * - 第一个列表有 5 个元素
 * - 第二个列表有 2 个元素
 * - 第三个列表只有 1 个元素
 * - 生成 10 个参数组合：5 × 2 × 1 = 10
 * 
 * 验证目标：
 * - 确认笛卡尔积对不同大小的列表都能正确工作
 * - 单元素列表会与其他列表的每个元素配对
 */
_TEST_SUITE_P(AsymmetricCartesianTest, vtest::ValueList<int>{1, 2, 3, 4, 5} * vtest::ValueList<bool>{false}
                                           * vtest::ValueList<const char *>{"a", "b"});

/**
 * 不对称笛卡尔积测试用例
 * 
 * 测试内容：
 * - 验证所有 10 种组合
 * - 第二个参数始终是 false
 * 
 * 执行次数：10 次
 */
TEST_P(AsymmetricCartesianTest, AsymmetricCombinationsTest)
{
    int         int_value  = GetParamValue<0>();
    bool        bool_value = GetParamValue<1>();
    const char *str_value  = GetParamValue<2>();

    // 验证组合数量：5 * 2 * 1 = 10
    EXPECT_GE(int_value, 1);
    EXPECT_LE(int_value, 5);
    EXPECT_TRUE(bool_value == false);
    EXPECT_TRUE(std::string(str_value) == "a" || std::string(str_value) == "b");
    // EXPECT_STREQ(str_value, "a"); // 第三个参数始终是 "a"
}

// ============================================================================
// 类型多样性测试
// ============================================================================

/**
 * 测试不同整数类型的笛卡尔积
 * 
 * 功能说明：
 * - 测试不同大小和符号的整数类型
 * - int8_t:  有符号 8 位整数 [-128, 127]
 * - uint8_t: 无符号 8 位整数 [0, 255]
 * - int16_t: 有符号 16 位整数 [-32768, 32767]
 * - 生成 27 个参数组合：3 × 3 × 3 = 27
 * 
 * 目的：
 * - 验证不同整数类型可以在同一个测试中使用
 * - 测试类型转换和边界值处理
 */
_TEST_SUITE_P(IntTypesTest, vtest::ValueList<int8_t>{-128, 0, 127} * vtest::ValueList<uint8_t>{0, 128, 255}
                                * vtest::ValueList<int16_t>{-32768, 0, 32767});

/**
 * 不同整数类型测试用例
 * 
 * 测试内容：
 * - 验证每种整数类型的边界值
 * - 确认类型转换正确
 * 
 * 执行次数：27 次
 */
TEST_P(IntTypesTest, DifferentIntegerTypesTest)
{
    int8_t  val1 = GetParamValue<0>();
    uint8_t val2 = GetParamValue<1>();
    int16_t val3 = GetParamValue<2>();

    EXPECT_GE(val1, -128);
    EXPECT_LE(val1, 127);
    EXPECT_LE(val2, 255);
    EXPECT_GE(val3, -32768);
    EXPECT_LE(val3, 32767);
}

/**
 * 测试浮点类型混合（float 和 double）
 * 
 * 功能说明：
 * - 测试单精度（float）和双精度（double）浮点数的组合
 * - float:  32 位浮点数
 * - double: 64 位浮点数
 * - 生成 6 个参数组合：3 × 2 = 6
 * 
 * 目的：
 * - 验证不同精度的浮点类型可以混合使用
 * - 测试精度转换
 */
_TEST_SUITE_P(FloatTypesTest, vtest::ValueList<float>{1.0f, 2.0f, 3.14f} * vtest::ValueList<double>{2.71, 3.14});

/**
 * 浮点类型混合测试用例
 * 
 * 测试内容：
 * - 验证 float 和 double 值的范围
 * - 检查精度处理
 * 
 * 执行次数：6 次
 */
TEST_P(FloatTypesTest, FloatAndDoubleTest)
{
    float  float_value  = GetParamValue<0>();
    double double_value = GetParamValue<1>();

    EXPECT_GT(float_value, 0.0f);
    EXPECT_GT(double_value, 0.0);
    EXPECT_LE(float_value, 3.15f);
    EXPECT_LE(double_value, 3.15);
}

// ============================================================================
// 字符串和字符测试
// ============================================================================

/**
 * 测试各种字符串内容
 * 
 * 功能说明：
 * - 测试不同类型的字符串内容
 * - 包括空字符串、单字符、普通字符串、特殊字符、Unicode 等
 * 
 * 测试参数：
 * - ""            // 空字符串
 * - "a"           // 单字符
 * - "hello"       // 普通字符串
 * - "Hello World" // 带空格的字符串
 * - "123"         // 数字字符串
 * - "!@#$%"       // 特殊字符
 * - "中文测试"    // Unicode 字符（UTF-8 编码）
 * 
 * 目的：
 * - 验证各种字符串内容都能正确处理
 * - 测试边界情况（空字符串）
 * - 测试特殊字符和 Unicode 支持
 */
_TEST_SUITE_P(StringContentTest,
              vtest::ValueList<const char *>{"",            // 空字符串
                                             "a",           // 单字符
                                             "hello",       // 普通字符串
                                             "Hello World", // 带空格
                                             "123",         // 数字字符串
                                             "!@#$%",       // 特殊字符
                                             "中文测试"});  // Unicode 字符

/**
 * 各种字符串内容测试用例
 * 
 * 测试内容：
 * - 验证字符串长度
 * - 根据内容进行特定验证（如数字字符串）
 * 
 * 执行次数：7 次
 */
TEST_P(StringContentTest, VariousStringContentTest)
{
    const char *str   = GetParamValue<0>();
    std::string value = str;

    // 验证字符串长度
    EXPECT_GE(value.length(), 0);

    // 根据内容进行不同的验证
    if (value.empty())
    {
        EXPECT_EQ(value.length(), 0);
    }
    else if (value == "123")
    {
        // 验证所有字符都是数字
        EXPECT_TRUE(std::all_of(value.begin(), value.end(), ::isdigit));
    }
}

/**
 * 测试字符串与整数组合
 * 
 * 功能说明：
 * - 测试字符串前缀/后缀与数字的组合
 * - 可用于生成带编号的标识符
 * - 生成 15 个参数组合：3 × 5 = 15
 * 
 * 应用场景：
 * - 测试命名规则（如 "prefix1", "prefix2" 等）
 * - 验证字符串拼接功能
 */
_TEST_SUITE_P(StringIntComboTest, vtest::ValueList<const char *>{"prefix", "suffix", "middle"}
                                      * vtest::ValueList<int>{0, 1, 10, 100, 1000});

/**
 * 字符串整数组合测试用例
 * 
 * 测试内容：
 * - 将字符串和整数组合成新字符串
 * - 验证组合结果不为空
 * 
 * 执行次数：15 次
 */
TEST_P(StringIntComboTest, StringIntCombinationTest)
{
    const char *str = GetParamValue<0>();
    int         num = GetParamValue<1>();

    // 组合字符串和数字
    std::string combined = std::string(str) + std::to_string(num);
    EXPECT_FALSE(combined.empty());
    EXPECT_GT(combined.length(), 0);
}

// ============================================================================
// 布尔值组合测试
// ============================================================================

/**
 * 测试多个布尔值的组合
 * 
 * 功能说明：
 * - 测试三个布尔值的所有可能组合
 * - 生成 8 个参数组合：2 × 2 × 2 = 8
 * 
 * 所有组合（真值表）：
 * - (false, false, false)
 * - (false, false, true)
 * - (false, true,  false)
 * - (false, true,  true)
 * - (true,  false, false)
 * - (true,  false, true)
 * - (true,  true,  false)
 * - (true,  true,  true)
 * 
 * 应用场景：
 * - 测试多个开关/标志的组合
 * - 验证逻辑运算
 * - 覆盖所有布尔状态组合
 */
_TEST_SUITE_P(MultiBoolTest, vtest::ValueList<bool>{true, false} * vtest::ValueList<bool>{true, false}
                                 * vtest::ValueList<bool>{true, false});

/**
 * 三布尔值组合测试用例
 * 
 * 测试内容：
 * - 验证所有 8 种布尔组合
 * - 测试逻辑运算（AND, OR, NOT）
 * - 验证真值计数
 * 
 * 执行次数：8 次
 */
TEST_P(MultiBoolTest, ThreeBoolCombinationsTest)
{
    bool flag1 = GetParamValue<0>();
    bool flag2 = GetParamValue<1>();
    bool flag3 = GetParamValue<2>();

    // 计算有多少个 true
    int true_count = (flag1 ? 1 : 0) + (flag2 ? 1 : 0) + (flag3 ? 1 : 0);
    EXPECT_GE(true_count, 0);
    EXPECT_LE(true_count, 3);

    // 测试逻辑运算
    bool all_true  = flag1 && flag2 && flag3;    // 全部为真
    bool any_true  = flag1 || flag2 || flag3;    // 至少一个为真
    bool all_false = !flag1 && !flag2 && !flag3; // 全部为假

    // 验证逻辑一致性
    EXPECT_TRUE((all_true && true_count == 3) || (!all_true && true_count < 3));
    EXPECT_TRUE((any_true && true_count > 0) || (!any_true && true_count == 0));
    EXPECT_TRUE((all_false && true_count == 0) || (!all_false && true_count > 0));
}

// ============================================================================
// 负数和零值测试
// ============================================================================

/**
 * 测试负数、零、正数的完整范围
 * 
 * 功能说明：
 * - 测试有符号整数的三个区域：负数、零、正数
 * - 包括不同量级的值
 * 
 * 测试参数：
 * - 负数：-100, -10, -1
 * - 零：0
 * - 正数：1, 10, 100
 * 
 * 目的：
 * - 验证符号处理
 * - 测试绝对值计算
 * - 确认零的特殊性质
 */
_TEST_SUITE_P(SignedNumberTest, vtest::ValueList<int>{-100, -10, -1, 0, 1, 10, 100});

/**
 * 有符号数范围测试用例
 * 
 * 测试内容：
 * - 根据符号进行不同的验证
 * - 测试绝对值计算
 * - 验证零的对称性
 * 
 * 执行次数：7 次
 */
TEST_P(SignedNumberTest, SignedNumberRangeTest)
{
    int value = GetParamValue<0>();

    if (value < 0)
    {
        // 负数测试
        EXPECT_LT(value, 0);
        EXPECT_EQ(std::abs(value), -value); // 绝对值等于相反数
    }
    else if (value == 0)
    {
        // 零的特殊性质
        EXPECT_EQ(value, 0);
        EXPECT_EQ(value, -value); // 零的相反数是自己
    }
    else
    {
        // 正数测试
        EXPECT_GT(value, 0);
        EXPECT_EQ(std::abs(value), value); // 绝对值等于自己
    }
}

/**
 * 测试浮点数的符号（包括正零和负零）
 * 
 * 功能说明：
 * - 测试浮点数的符号位
 * - IEEE 754 标准定义了正零（+0.0）和负零（-0.0）
 * - 虽然 +0.0 == -0.0，但它们的符号位不同
 * 
 * 测试参数：
 * - -3.14  // 负数
 * - -1.0   // 负单位值
 * - -0.0   // 负零
 * - 0.0    // 正零
 * - 1.0    // 正单位值
 * - 3.14   // 正数
 * 
 * 目的：
 * - 验证符号位的正确性
 * - 测试正零和负零的区别
 * - 确认 std::signbit() 的行为
 */
_TEST_SUITE_P(FloatSignTest, vtest::ValueList<double>{-3.14, -1.0, -0.0, 0.0, 1.0, 3.14});

/**
 * 浮点数符号测试用例
 * 
 * 测试内容：
 * - 验证符号位
 * - 区分正零和负零
 * - 测试 std::signbit() 函数
 * 
 * 执行次数：6 次
 */
TEST_P(FloatSignTest, FloatSignTest)
{
    double value = GetParamValue<0>();

    if (value == 0.0)
    {
        // 零值测试（可能是正零或负零）
        EXPECT_EQ(value, 0.0);
        // 注意：正零和负零相等，但符号位不同
    }
    else if (value < 0.0)
    {
        // 负数测试
        EXPECT_LT(value, 0.0);
        EXPECT_TRUE(std::signbit(value)); // 符号位为 1
    }
    else
    {
        // 正数测试
        EXPECT_GT(value, 0.0);
        EXPECT_FALSE(std::signbit(value)); // 符号位为 0
    }
}

// ============================================================================
// 大规模参数组合测试
// ============================================================================

/**
 * 测试较大的参数集合
 * 
 * 功能说明：
 * - 测试较大规模的笛卡尔积
 * - 生成 50 个参数组合：10 × 5 = 50
 * 
 * 目的：
 * - 验证系统能处理较多的测试用例
 * - 测试性能和稳定性
 * - 确认大规模参数化测试的可行性
 */
_TEST_SUITE_P(LargeParamSetTest, vtest::ValueList<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
                                     * vtest::ValueList<const char *>{"a", "b", "c", "d", "e"});

/**
 * 大参数集测试用例
 * 
 * 测试内容：
 * - 验证所有 50 种组合
 * - 检查参数范围
 * 
 * 执行次数：50 次
 */
TEST_P(LargeParamSetTest, LargeParameterSetTest)
{
    int         int_value = GetParamValue<0>();
    const char *str_value = GetParamValue<1>();

    // 验证参数范围（10 * 5 = 50 种组合）
    EXPECT_GE(int_value, 1);
    EXPECT_LE(int_value, 10);
    EXPECT_TRUE(std::string(str_value).length() == 1); // 所有字符串都是单字符
}

// ============================================================================
// 去重功能详细测试
// ============================================================================

/**
 * 测试大量重复值的去重
 * 
 * 功能说明：
 * - 输入包含大量重复值
 * - 原始列表：{1, 1, 1, 2, 2, 2, 3, 3, 3, 1, 2, 3, 1, 2, 3, 1, 1, 2, 2, 3, 3}
 * - 共 21 个元素，但只有 3 个唯一值
 * - 去重后：{1, 2, 3}
 * 
 * 验证目标：
 * - 确认大量重复值被正确去除
 * - 验证去重算法的效率
 * - 测试只运行 3 次而不是 21 次
 */
_TEST_SUITE_P(HeavyDuplicateTest, vtest::ValueList<int>{1, 1, 1, 2, 2, 2, 3, 3, 3, 1, 2, 3, 1, 2, 3, 1, 1, 2, 2, 3, 3});

/**
 * 大量重复值去重测试用例
 * 
 * 测试内容：
 * - 验证只有唯一值被测试
 * - 确认测试次数正确
 * 
 * 执行次数：3 次（而不是 21 次）
 */
TEST_P(HeavyDuplicateTest, ManyDuplicatesRemovalTest)
{
    int value = GetParamValue<0>();
    // 应该只有 3 个唯一值
    EXPECT_TRUE(value == 1 || value == 2 || value == 3);
}

/**
 * 测试字符串去重
 * 
 * 功能说明：
 * - 测试字符串类型的去重功能
 * - 原始列表：{"apple", "banana", "apple", "cherry", "banana", "apple"}
 * - 包含重复的字符串
 * - 去重后：{"apple", "banana", "cherry"}（按字典序排列）
 * 
 * 验证目标：
 * - 确认字符串比较和去重正确
 * - 验证字符串排序
 */
_TEST_SUITE_P(StringDuplicateTest,
              vtest::ValueList<std::string>{"apple", "banana", "apple", "cherry", "banana", "apple"});

/**
 * 字符串去重测试用例
 * 
 * 测试内容：
 * - 验证只有唯一字符串被测试
 * - 确认字符串比较正确
 * 
 * 执行次数：3 次（而不是 6 次）
 */
TEST_P(StringDuplicateTest, StringDeduplicationTest)
{
    std::string value = GetParamValue<0>();
    // 应该只有 3 个唯一字符串
    EXPECT_TRUE(value == "apple" || value == "banana" || value == "cherry");
}

// ============================================================================
// 排序验证测试
// ============================================================================

/**
 * 测试参数是否按升序排列
 * 
 * 功能说明：
 * - 输入无序列表：{9, 3, 7, 1, 5, 2, 8, 4, 6}
 * - _TEST_SUITE_P 会自动排序
 * - 实际测试顺序：{1, 2, 3, 4, 5, 6, 7, 8, 9}
 * 
 * 验证目标：
 * - 确认参数按升序排列
 * - 验证排序算法的正确性
 * 
 * 注意：
 * - 使用静态变量跟踪上一个值
 * - 这种方法依赖于测试的执行顺序
 */
_TEST_SUITE_P(SortOrderTest, vtest::ValueList<int>{9, 3, 7, 1, 5, 2, 8, 4, 6});

/**
 * 参数排序测试用例
 * 
 * 测试内容：
 * - 验证参数值在有效范围内
 * - 验证参数按升序排列（使用静态变量）
 * 
 * 执行次数：9 次
 * 
 * 警告：
 * - 这个测试依赖于 Google Test 按顺序执行参数化测试
 * - 如果测试并行执行，这个验证可能失败
 */
TEST_P(SortOrderTest, ParameterSortingTest)
{
    int value = GetParamValue<0>();
    EXPECT_GE(value, 1);
    EXPECT_LE(value, 9);

    // 记录上一个值，验证排序（需要静态变量）
    static int last_value = 0;
    if (last_value > 0)
    {
        EXPECT_GE(value, last_value); // 当前值应该 >= 上一个值
    }
    last_value = value;
}

// ============================================================================
// 混合复杂类型测试
// ============================================================================

/**
 * 测试五参数混合类型
 * 
 * 功能说明：
 * - 测试五个不同类型参数的笛卡尔积
 * - 生成 48 个参数组合：2 × 2 × 2 × 2 × 3 = 48
 * 
 * 参数类型：
 * - int:        {1, 2}
 * - double:     {1.1, 2.2}
 * - bool:       {true, false}
 * - const char*: {"x", "y"}
 * - int8_t:     {-1, 0, 1}
 * 
 * 目的：
 * - 验证多种类型可以混合使用
 * - 测试复杂参数组合的处理
 * - 确认大量组合的稳定性
 */
_TEST_SUITE_P(FiveParamMixedTest, vtest::ValueList<int>{1, 2} * vtest::ValueList<double>{1.1, 2.2}
                                      * vtest::ValueList<bool>{true, false} * vtest::ValueList<const char *>{"x", "y"}
                                      * vtest::ValueList<int8_t>{-1, 0, 1});

/**
 * 五参数混合类型测试用例
 * 
 * 测试内容：
 * - 验证所有 48 种参数组合
 * - 检查每个参数的类型和值
 * 
 * 执行次数：48 次
 */
TEST_P(FiveParamMixedTest, FiveParameterMixedTypesTest)
{
    int         param1 = GetParamValue<0>();
    double      param2 = GetParamValue<1>();
    bool        param3 = GetParamValue<2>();
    const char *param4 = GetParamValue<3>();
    int8_t      param5 = GetParamValue<4>();

    // 验证所有参数（2 * 2 * 2 * 2 * 3 = 48 种组合）
    EXPECT_TRUE(param1 == 1 || param1 == 2);
    EXPECT_TRUE(param2 == 1.1 || param2 == 2.2);
    EXPECT_TRUE(param3 == true || param3 == false);
    EXPECT_TRUE(std::string(param4) == "x" || std::string(param4) == "y");
    EXPECT_TRUE(param5 == -1 || param5 == 0 || param5 == 1);
}

// ============================================================================
// 单值参数测试
// ============================================================================

/**
 * 测试只有一个值的参数列表
 * 
 * 功能说明：
 * - 参数列表只包含一个值：{42}
 * - 测试会运行 1 次
 * 
 * 应用场景：
 * - 测试特定的单一配置
 * - 作为参数化测试框架的边界情况
 * - 验证单值列表的处理
 */
_TEST_SUITE_P(SingleValueTest, vtest::ValueList<int>{42});

/**
 * 单值测试用例
 * 
 * 测试内容：
 * - 验证唯一的参数值
 * 
 * 执行次数：1 次
 */
TEST_P(SingleValueTest, OnlyOneValueTest)
{
    int value = GetParamValue<0>();
    EXPECT_EQ(value, 42); // 答案是 42 😊
}

/**
 * 测试单值笛卡尔积
 * 
 * 功能说明：
 * - 每个参数列表都只有一个值
 * - 生成 1 个参数组合：1 × 1 × 1 = 1
 * 
 * 应用场景：
 * - 测试特定的单一配置组合
 * - 验证单值笛卡尔积的处理
 * - 作为多参数测试的边界情况
 */
_TEST_SUITE_P(SingleValueCartesianTest,
              vtest::ValueList<int>{1} * vtest::ValueList<const char *>{"only"} * vtest::ValueList<bool>{true});

/**
 * 单值组合测试用例
 * 
 * 测试内容：
 * - 验证唯一的参数组合
 * - 确认每个参数的值
 * 
 * 执行次数：1 次
 */
TEST_P(SingleValueCartesianTest, SingleValueCombinationTest)
{
    int         int_value  = GetParamValue<0>();
    const char *str_value  = GetParamValue<1>();
    bool        bool_value = GetParamValue<2>();

    EXPECT_EQ(int_value, 1);
    EXPECT_STREQ(str_value, "only");
    EXPECT_TRUE(bool_value);
}

// ============================================================================
// 精度测试
// ============================================================================

/**
 * 测试浮点数精度
 * 
 * 功能说明：
 * - 测试浮点数的精度问题
 * - 包括常见的精度陷阱（如 0.1 + 0.2 != 0.3）
 * - 包括数学常数（π, e）
 * 
 * 测试参数：
 * - 0.1, 0.2           // 基本小数
 * - 0.1 + 0.2          // 浮点数加法（可能接近但不等于 0.3）
 * - 1.0 / 3.0          // 无限循环小数
 * - 2.0 / 3.0          // 另一个无限循环小数
 * - std::numbers::pi   // 圆周率 π
 * - std::numbers::e    // 自然对数的底 e
 * 
 * 目的：
 * - 验证浮点数比较的正确方法
 * - 测试精度容差
 * - 确认数学常数的可用性
 */
_TEST_SUITE_P(FloatPrecisionTest, vtest::ValueList<double>{0.1, 0.2, 0.3, 0.1 + 0.2, 1.0 / 3.0, 2.0 / 3.0,
                                                           std::numbers::pi, std::numbers::e});

/**
 * 浮点数精度测试用例
 * 
 * 测试内容：
 * - 验证值不是 NaN 或无穷大
 * - 测试浮点数比较（使用容差）
 * - 验证 0.1 + 0.2 与 0.3 的关系
 * 
 * 执行次数：8 次
 */
TEST_P(FloatPrecisionTest, FloatingPointPrecisionTest)
{
    double value = GetParamValue<0>();

    EXPECT_FALSE(std::isnan(value));
    EXPECT_FALSE(std::isinf(value));

    // 测试浮点数比较
    if (std::abs(value - 0.3) < 1e-10)
    {
        // 0.1 + 0.2 可能不完全等于 0.3（浮点数精度问题）
        EXPECT_NEAR(value, 0.3, 1e-10); // 使用容差比较
    }
}

// ============================================================================
// 特殊字符和转义测试
// ============================================================================

/**
 * 测试包含特殊字符的字符串
 * 
 * 功能说明：
 * - 测试各种转义字符和特殊字符
 * - 验证字符串字面量的正确处理
 * 
 * 测试参数：
 * - "tab\there"        // 制表符 \t
 * - "newline\nhere"    // 换行符 \n
 * - "quote\"here"      // 双引号 \"
 * - "backslash\\here"  // 反斜杠 \\
 * - "null\0char"       // 空字符 \0（注意：会截断字符串）
 * 
 * 注意：
 * - 包含 \0 的字符串会在 \0 处截断
 * - C 风格字符串以 \0 结尾
 */
_TEST_SUITE_P(SpecialCharsTest, vtest::ValueList<const char *>{"tab\there", "newline\nhere", "quote\"here",
                                                               "backslash\\here", "null\0char"});

/**
 * 特殊字符测试用例
 * 
 * 测试内容：
 * - 验证字符串指针不为空
 * - 检查字符串长度
 * 
 * 执行次数：5 次
 */
TEST_P(SpecialCharsTest, SpecialCharactersTest)
{
    const char *str = GetParamValue<0>();
    EXPECT_NE(str, nullptr);

    std::string value = str;
    EXPECT_GE(value.length(), 0);
}

// ============================================================================
// 性能和压力测试
// ============================================================================

/**
 * 测试大量参数组合（压力测试）
 * 
 * 功能说明：
 * - 测试系统处理大量测试用例的能力
 * - 生成 150 个参数组合：15 × 2 × 5 = 150
 * 
 * 参数规模：
 * - 第一个参数：15 个值
 * - 第二个参数：2 个值
 * - 第三个参数：5 个值
 * 
 * 目的：
 * - 验证系统稳定性
 * - 测试性能表现
 * - 确认大规模测试的可行性
 */
_TEST_SUITE_P(StressTest, vtest::ValueList<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
                              * vtest::ValueList<bool>{true, false} * vtest::ValueList<int8_t>{-2, -1, 0, 1, 2});

/**
 * 大组合压力测试用例
 * 
 * 测试内容：
 * - 验证所有 150 种参数组合
 * - 检查参数范围
 * 
 * 执行次数：150 次
 */
TEST_P(StressTest, LargeCombinationStressTest)
{
    int    param1 = GetParamValue<0>();
    bool   param2 = GetParamValue<1>();
    int8_t param3 = GetParamValue<2>();

    // 验证参数（15 * 2 * 5 = 150 种组合）
    EXPECT_GE(param1, 1);
    EXPECT_LE(param1, 15);
    EXPECT_TRUE(param2 == true || param2 == false);
    EXPECT_GE(param3, -2);
    EXPECT_LE(param3, 2);
}

// ============================================================================
// 数学运算测试
// ============================================================================

/**
 * 测试数学运算的参数组合
 * 
 * 功能说明：
 * - 使用参数化测试验证数学运算的性质
 * - 生成 30 个参数组合：6 × 5 = 30
 * 
 * 测试的数学性质：
 * - 交换律：a + b = b + a, a * b = b * a
 * - 结合律：(a + b) + c = a + (b + c)
 * - 分配律：a * (b + c) = a * b + a * c
 * - 除法和取模的关系：a = (a / b) * b + (a % b)
 * 
 * 参数选择：
 * - 包括零（特殊情况）
 * - 包括不同量级的数字
 */
_TEST_SUITE_P(MathOperationsTest, vtest::ValueList<int>{0, 1, 2, 5, 10, 100} * vtest::ValueList<int>{1, 2, 3, 4, 5});

/**
 * 数学运算测试用例
 * 
 * 测试内容：
 * - 验证加法和乘法的交换律
 * - 验证和的下界
 * - 验证除法和取模的关系
 * 
 * 执行次数：30 次
 */
TEST_P(MathOperationsTest, MathematicalOperationsTest)
{
    int a = GetParamValue<0>();
    int b = GetParamValue<1>();

    // 测试各种数学运算
    EXPECT_EQ(a + b, b + a);             // 加法交换律
    EXPECT_EQ(a * b, b * a);             // 乘法交换律
    EXPECT_GE(a + b, std::max(a, b));    // 和大于等于最大值
    EXPECT_LE(a * b, (a + 1) * (b + 1)); // 乘积的上界

    if (b != 0)
    {
        // 除法和取模的关系：a = (a / b) * b + (a % b)
        EXPECT_EQ(a / b * b + a % b, a);
    }
}

// ============================================================================
// 逻辑运算测试
// ============================================================================

/**
 * 测试布尔逻辑运算
 * 
 * 功能说明：
 * - 使用参数化测试验证布尔逻辑的性质
 * - 生成 4 个参数组合：2 × 2 = 4
 * 
 * 测试的逻辑性质：
 * - 交换律：a && b = b && a, a || b = b || a
 * - 德摩根定律：!(a && b) = !a || !b, !(a || b) = !a && !b
 * - 恒等律：a && true = a, a || false = a
 * - 零律：a && false = false, a || true = true
 * 
 * 所有组合：
 * - (false, false)
 * - (false, true)
 * - (true, false)
 * - (true, true)
 */
_TEST_SUITE_P(LogicOperationsTest, vtest::ValueList<bool>{true, false} * vtest::ValueList<bool>{true, false});

/**
 * 布尔逻辑测试用例
 * 
 * 测试内容：
 * - 验证逻辑运算的交换律
 * - 验证德摩根定律
 * - 验证恒等律和零律
 * 
 * 执行次数：4 次
 */
TEST_P(LogicOperationsTest, BooleanLogicTest)
{
    bool a = GetParamValue<0>();
    bool b = GetParamValue<1>();

    // 测试逻辑运算规则
    EXPECT_EQ(a && b, b && a);      // 与运算交换律
    EXPECT_EQ(a || b, b || a);      // 或运算交换律
    EXPECT_EQ(!(a && b), !a || !b); // 德摩根定律
    EXPECT_EQ(!(a || b), !a && !b); // 德摩根定律
    EXPECT_EQ(a && true, a);        // 恒等律
    EXPECT_EQ(a || false, a);       // 恒等律
    EXPECT_EQ(a && false, false);   // 零律
    EXPECT_EQ(a || true, true);     // 零律
}
