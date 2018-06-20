#include "obd.hpp"
#include "cstring"  // for memcpy
#include <iostream> // types


/*
* @brief Построить деревья для синтаксического анализа
*/
void OBD::CreateTrees()
{
    for (uint8_t paramCount = 0; paramCount < ParamNumber; paramCount++)
    {
        uint8_t dataByteCount = 0;
        memset(ParamTable[paramCount].DataBytes, 0x55, 4);
        for (uint8_t byteCount = ParamTable[paramCount].FormulaLength; byteCount > 1;)
        {
            uint8_t byte = ParamTable[paramCount].Formula[--byteCount];
            if ( IsItOperand(byte) )
            {
                Tree::Node* op = ParamTable[paramCount].tree.AddNodeLower(byte);
				stack.Push(op);
                if ( IsItDataFrame(byte) && (dataByteCount < MAX_NUMBER_OF_DATA_BYTES) &&
                    (byte != ParamTable[paramCount].DataBytes[0]) && (ParamTable[paramCount].DataBytes[1]) &&
                    (byte != ParamTable[paramCount].DataBytes[2]) && (ParamTable[paramCount].DataBytes[3]) )
                    ParamTable[paramCount].DataBytes[dataByteCount++] = byte;
            }
            else /// (IsItOperator(byte) )
            {
                if ( IsItUnaryOperator(byte) )
                {
                    uint8_t length = 1;
                    Tree::Node* ops[3];
                    ops[0] = stack.Pop();
                    Tree::Node* op = ParamTable[paramCount].tree.AddNodeParent(byte, ops, length);
					stack.Push(op);
                }
                else if ( IsItBinaryOperator(byte) )
                {
                    uint8_t length = 2;
                    Tree::Node* ops[3];
                    ops[0] = stack.Pop();
                    ops[1] = stack.Pop();
                    Tree::Node* op = ParamTable[paramCount].tree.AddNodeParent(byte, ops, length);
					stack.Push(op);
                }
                else if ( IsItTernaryOperator(byte) )
                {
                    uint8_t length = 3;
                    Tree::Node* ops[3];
                    ops[0] = stack.Pop();
                    ops[1] = stack.Pop();
                    ops[2] = stack.Pop();
                    Tree::Node* op = ParamTable[paramCount].tree.AddNodeParent(byte, ops, length);
					stack.Push(op);
                }
                else
                {

                }

            }
        }
    }
}


/*
* @brief Выполнение прямого расчета по формуле
* @note скопировано из терминала
*/
void OBD::DoDirectCalculate()
{
    uint8_t param = ParamCount;
    uint8_t operators[16];
    uint32_t operands[16];
    int8_t operatorIndex = -1;
    int8_t operandIndex = -1;

    bool ignore = false;

    for (uint8_t index = ParamTable[param].FormulaLength - 1; index >= 1; index--)
    {
        uint8_t byte = ParamTable[param].Formula[index];

        /// Если байт данных фрейма
        if (byte < 8)
        {
            operands[++operandIndex] = frame.Data[byte];
        }
        /// Если оператор
        else if ((byte >= 8) && (byte <= 127))
        {
            operators[++operatorIndex] = byte;
        }
        /// Если операнд
        else
        {
            operands[++operandIndex] = (byte & 0x7F);
        }

        /// Основной алгоритм
        if ((operatorIndex >= 0) && (operandIndex >= 1))
        {
            if ((operators[operatorIndex] == OPCODE_LOG_NOT) || (operators[operatorIndex] == OPCODE_BIT_NOT))
            {
                uint32_t result = CalculateDirectElementary(operators[operatorIndex], operands[operandIndex]);
                operatorIndex -= 1;
                operands[operandIndex] = result;
            }
            else if ((operators[operatorIndex] == OPCODE_IF_ELSE) && (operandIndex == 2))
            {
                uint32_t result = CalculateDirectElementary(operators[operatorIndex], operands[operandIndex], operands[operandIndex - 1], operands[operandIndex - 2]);
                if (result == 0xFFFFFFFF)
                {
                    ignore = true;
                    break;
                }
                operatorIndex -= 1;
                operandIndex -= 2;
                operands[operandIndex] = result;
            }
            else
            {
                uint32_t result = CalculateDirectElementary(operators[operatorIndex], operands[operandIndex], operands[operandIndex - 1]);
                operatorIndex -= 1;
                operandIndex -= 1;
                operands[operandIndex] = result;
            }
        }
    }
    Value = operands[0];
}


/*
* @brief Выполнить попытку рекурсивного обратного расчета с помощью дерева
* @param value - результат выполнения прямого расчета указанного узла (оператора)
* @return value - рассчитанное число
*/
uint32_t OBD::DoReverseCalculateWithTree(uint32_t value, const Tree::Node* node)
{
    enum: uint8_t
    {
        OK = 0,
        FORMULA_ERROR_NODE_IS_OPERAND = 1,
        BASE_IS_NULL = 2,
        UNEXPECTED_ERROR = 255,
    };
    Tree::Node* base = ParamTable[ParamCount].tree.GetBaseNode();
    static uint8_t status = OK;

    /// 0. Check correctness:
    if (node == nullptr)
    {
        if (base == nullptr)
            return BASE_IS_NULL;
        node = base;
        status = OK;
    }
    if (status != OK)
        return 0;

    /// 1. If this node is OPERAND => this node is apex (висячая вершина)
    if (node->ChildsCount == 0)
    {
        if ((node == base) && IsItDataFrame(node->Value) )
        {
            frame.Data[node->Value] = value;
        }
        else
        {
            status = FORMULA_ERROR_NODE_IS_OPERAND;
        }
    }
    /// 2. If this node is UNARY OPERATOR, try to calculate it
    else if (node->ChildsCount == 1)
    {
        uint8_t childByte = base->ChildsArr[0]->Value;

        if (IsItOperand(childByte))
        {
            value = CalculateReverseElementary(value, node->Value, childByte);
        }
        else /// ( IsItOperator(childByte) )
        {
            value = DoReverseCalculateWithTree(value, node->ChildsArr[0]);
        }
    }
    /// 3. If this node is BINARY OPERATOR, try to calculate it
    else if (node->ChildsCount == 2)
    {
        uint8_t operand1 = node->ChildsArr[0]->Value;
        uint8_t operand2 = node->ChildsArr[1]->Value;

        if ( IsItConst(operand1) && IsItConst(operand2) )
        {
            operand1 -= 0x80;   /// from byteConst to operandConst
            operand2 -= 0x80;   /// from byteConst to operandConst
            value = CalculateDirectElementary(node->Value, operand1, operand2);
        }
        else if ( IsItOperand(operand1) || IsItOperand(operand2) )
        {
            value = CalculateReverseElementary(value, node->Value, operand1, operand2);
            if ( IsItOperator(operand1) )
                value = DoReverseCalculateWithTree(value, node->ChildsArr[0]);
            else if( IsItOperator(operand2) )
                value = DoReverseCalculateWithTree(value, node->ChildsArr[1]);
        }
        else if ( IsItOperator(operand1) && IsItOperator(operand2) )
        {
            operand1 = DoReverseCalculateWithTree(value, node->ChildsArr[0]);
            operand2 = DoReverseCalculateWithTree(value, node->ChildsArr[1]);
            value = CalculateDirectElementary(value, node->Value, operand1, operand2);
        }
    }
    /// 4. If this node is TERNARY OPERATOR, try to calculate it
    else if (node->ChildsCount == 3)
    {
        /*There are no such formulas*/
    }
    /// 5. Если текущий узел - не пойми что
    else
        status = UNEXPECTED_ERROR;
    return value;
}


/*
* @brief Обратный расчет с помощью метода перебора
* @note Может выполняться очень долго (до получаса при 4 байтах),
* поэтому при 4-ех байтах возвращаем ошибку
* @param needValue - число, которое должен распарсить терминал
* @return статус выполнения: 0 - все хорошо, иначе ошибка
*/
uint8_t OBD::DoReverseCalculateWithBruteForce(const int64_t needValue)
{
    enum : uint8_t
    {
        OK = 0,
        ERROR = 1,
    };
    uint8_t indexByte0 = ParamTable[ParamCount].DataBytes[0];
    uint8_t indexByte1 = ParamTable[ParamCount].DataBytes[1];
    uint8_t indexByte2 = ParamTable[ParamCount].DataBytes[2];
    uint8_t indexByte3 = ParamTable[ParamCount].DataBytes[3];

    if( IsItDataFrame(indexByte3) )
        return ERROR;

    uint16_t valueByte0 = ( IsItDataFrame(indexByte0) ) ? 0 : 255;
    uint16_t valueByte1 = ( IsItDataFrame(indexByte1) ) ? 0 : 255;
    uint16_t valueByte2 = ( IsItDataFrame(indexByte2) ) ? 0 : 255;
    uint16_t valueByte3 = ( IsItDataFrame(indexByte3) ) ? 0 : 255;
    while (valueByte3 < 256)
    {
        frame.Data[indexByte3] = (uint8_t)valueByte3;
        while (valueByte2 < 256)
        {
            frame.Data[indexByte2] = (uint8_t)valueByte2;
            while (valueByte1 < 256)
            {
                frame.Data[indexByte1] = (uint8_t)valueByte1;
                while (valueByte0 < 256)
                {
                    frame.Data[indexByte0] = (uint8_t)valueByte0;
                    DoDirectCalculate();
                    if (Value == needValue)
                        return OK;
                    valueByte0++;
                }
                valueByte0 = 0;
                valueByte1++;
            }
            valueByte1 = 0;
            valueByte2++;
        }
        valueByte2 = 0;
        valueByte3++;
    }
    return ERROR;
}


/*
* @brief Обратный расчет с помощью метода дихотомии
* @note Может не сойтись и зависнуть, однако относительно быстрый,
* поэтому добавлен счетчик, по переполнению которого возвращаем ошибку
* @param NeedValue - число, которое должен распарсить терминал
* @return статус выполнения: 0 - все хорошо, иначе ошибка
*/
uint8_t OBD::DoReverseCalculateWithMethodDichotomy(const int64_t NeedValue)
{
    enum : uint8_t
    {
        OK = 0,
        ERROR = 1,
    };

    /// Init variables
    uint32_t dataMinBorder = 0;
    uint32_t dataMaxBorder;
    uint32_t dataGuess;
    uint8_t counter = 0;
    uint8_t indexByte0 = ParamTable[ParamCount].DataBytes[0];
    uint8_t indexByte1 = ParamTable[ParamCount].DataBytes[1];
    uint8_t indexByte2 = ParamTable[ParamCount].DataBytes[2];
    uint8_t indexByte3 = ParamTable[ParamCount].DataBytes[3];
    if (IsItDataFrame(indexByte3))
    {
        dataMaxBorder = 4294967295;	/// 2^32 - 1
        dataGuess = 2147483648;		/// 2^31
        frame.Data[indexByte3] = (dataGuess >> 24) % 256;
        frame.Data[indexByte2] = (dataGuess >> 16) % 256;
        frame.Data[indexByte1] = (dataGuess >> 8) % 256;
        frame.Data[indexByte0] = dataGuess % 256;
    }
    else if (IsItDataFrame(indexByte2))
    {
        dataMaxBorder = 16777215;	/// 2^24 - 1
        dataGuess = 8388608;		/// 2^24
        frame.Data[indexByte2] = (dataGuess >> 16) % 256;
        frame.Data[indexByte1] = (dataGuess >> 8) % 256;
        frame.Data[indexByte0] = dataGuess % 256;
    }
    else if (IsItDataFrame(indexByte1))
    {
        dataMaxBorder = 65535;		/// 2^16 - 1
        dataGuess = 32769;			/// 2^16
        frame.Data[indexByte1] = (dataGuess >> 8) % 256;
        frame.Data[indexByte0] = dataGuess % 256;
    }
    else if (IsItDataFrame(indexByte0))
    {
        dataMaxBorder = 255;		/// 2^8 - 1
        dataGuess = 128;			/// 2^7
        frame.Data[indexByte0] = dataGuess;
    }
    else
        return ERROR;

    /// Main algorithm
    DoDirectCalculate();
    while (Value != NeedValue)
    {
        int64_t dif = Value - NeedValue;
        /// If the desired number on the left
        if (dif > 0)
        {
            dataMaxBorder = dataGuess;
            dataGuess = (dataMaxBorder + dataMinBorder) >> 1;	// round down
        }
        /// If the desired number on the right
        else
        {
            dataMinBorder = dataGuess;
            dataGuess = ((dataMaxBorder + dataMinBorder) >> 1) + 1;	// round upwards
        }
        frame.Data[indexByte0] = dataGuess % 256;
        frame.Data[indexByte1] = (dataGuess >> 8) % 256;
        frame.Data[indexByte2] = (dataGuess >> 16) % 256;
        frame.Data[indexByte3] = (dataGuess >> 24) % 256;
        DoDirectCalculate();

        /// Verification in case this method gets stuck
        if ((counter++) > 32)
        {
            return ERROR;
        }

    }
    return OK;
}


/*
* @brief Расчет выражения
* @note Скопировано из тестового стенда
* @param opcode - оператор
* @param operand1 - первый оператор
* @param operand2 - второй оператор, если есть
* @param operand3 - второй оператор, если есть
* @return - результат расчета выражения
*/
uint32_t OBD::CalculateDirectElementary(const uint8_t opcode, const uint32_t operand1, const uint32_t operand2, const uint32_t operand3)
{
    if (opcode == OPCODE_LOG_OR)			return operand1 || operand2;
    else if (opcode == OPCODE_LOG_AND)		return operand1 && operand2;
    else if (opcode == OPCODE_LOG_NOT)		return !operand1;
    else if (opcode == OPCODE_BIT_NOT)		return ~operand1;
    else if (opcode == OPCODE_BIT_OR)		return operand1 | operand2;
    else if (opcode == OPCODE_BIT_XOR)		return operand1 ^ operand2;
    else if (opcode == OPCODE_BIT_AND)		return operand1 & operand2;
    else if (opcode == OPCODE_EQU)			return operand1 == operand2;
    else if (opcode == OPCODE_NEQU)			return operand1 != operand2;
    else if (opcode == OPCODE_LESS)			return operand1 < operand2;
    else if (opcode == OPCODE_MORE)			return operand1 > operand2;
    else if (opcode == OPCODE_LESS_EQU)		return operand1 <= operand2;
    else if (opcode == OPCODE_MORE_EQU)		return operand1 >= operand2;
    else if (opcode == OPCODE_SHIFT_LEFT)	return operand1 << operand2;
    else if (opcode == OPCODE_SHIFT_RIGHT)	return operand1 >> operand2;
    else if (opcode == OPCODE_ADD)			return operand1 + operand2;
    else if (opcode == OPCODE_SUB)			return operand1 - operand2;
    else if (opcode == OPCODE_MUL)			return operand1 * operand2;
    else if (opcode == OPCODE_DIV)			return operand1 / operand2;
    else if (opcode == OPCODE_IF_ELSE)
    {
        if (operand1)
        {
            return operand2;
        }
        else
        {
            return 0xFFFFFFFF;
        }
    }
    return 0;
}


/*
* @brief Определение байтов фрейма по формуле и по конечному значению
* @param value - значение, полученное после подстановки переменных формулы
* @param opcode - оператор
* @param operand1 - первый оператор
* @param operand2 - второй оператор, если есть
* @param operand3 - второй оператор, если есть
* @note Предполагается, что хотя бы 1 из аргументов константа или байт данных фрейма
* @return partOfUnknownOperand - незивестный операнд
*/
uint32_t OBD::CalculateReverseElementary(uint32_t value, const uint8_t opcode, const uint32_t operand1, const uint32_t operand2, const uint32_t operand3)
{
    uint32_t unknownValue = 0;

    if ( IsItUnaryOperator(opcode) )
    {
        unknownValue = CalculateDirectElementary(opcode, value);
    }
    else if ( IsItBinaryOperator(opcode) )
    {
        /// 1. Выполнение обратного элементарного расчета, если есть хотя бы одна константа
        if ( IsItConst(operand1) || IsItConst(operand2) )
        {
            uint8_t knownConstant = ( IsItConst(operand1) ) ? (operand1 - 0x80) : (operand2 - 0x80);

            /// 1.1. Выполнение расчета
            if (opcode == OPCODE_LOG_OR)            unknownValue = (value) ? 1 : 0;
            else if (opcode == OPCODE_LOG_AND)      unknownValue = (value) ? 1 : 0;
            else if (opcode == OPCODE_BIT_OR)       unknownValue = (value&(~knownConstant));
            else if (opcode == OPCODE_BIT_XOR)      {/*in practice does not occur*/}
            else if (opcode == OPCODE_BIT_AND)      unknownValue = (value&knownConstant);
            else if (opcode == OPCODE_EQU)          unknownValue = (value) ? knownConstant : !knownConstant;
            else if (opcode == OPCODE_NEQU)         unknownValue = (value) ? !knownConstant : knownConstant;
            else if (opcode == OPCODE_LESS)			{/*in practice does not occur*/ }
            else if (opcode == OPCODE_MORE)			{/*in practice does not occur*/ }
            else if (opcode == OPCODE_LESS_EQU)		{/*in practice does not occur*/ }
            else if (opcode == OPCODE_MORE_EQU)		{/*in practice does not occur*/ }
            else if (opcode == OPCODE_SHIFT_LEFT)   unknownValue = value >> knownConstant;
            else if (opcode == OPCODE_SHIFT_RIGHT)  unknownValue = value << knownConstant;
            else if (opcode == OPCODE_ADD)          unknownValue = value - knownConstant;
            else if (opcode == OPCODE_SUB)          // dependent
            {
                if( IsItConst(operand2) )           unknownValue = value + knownConstant;
                else                                unknownValue = knownConstant - value;
            }
            else if (opcode == OPCODE_MUL)          unknownValue = value / knownConstant;
            else if (opcode == OPCODE_DIV)          // dependent
            {
                if( IsItConst(operand2) )           unknownValue = value * knownConstant;
                else                                unknownValue = knownConstant / value;
            }
            else
                return 0;

            /// 1.2. Заполнение байт данных фрейма соответствующими значениями
            if ( opcode == OPCODE_EQU ) /// для "==" заполняем особенным образом
            {
                if (IsItDataFrame(operand1))
                    frame.Data[operand1] = unknownValue;
                else if (IsItDataFrame(operand2))
                    frame.Data[operand2] = unknownValue;
            }
            else
            {
                if (IsItDataFrame(operand1))
                    frame.Data[operand1] |= unknownValue;
                else if (IsItDataFrame(operand2))
                    frame.Data[operand2] |= unknownValue;
            }
        }

        /// 2. Попытка выполнения обратного элементарного расчета, если константы нет
        else if ( IsItDataFrame(operand1) || IsItDataFrame(operand2) )
        {
            uint8_t byteFrameCount = ( IsItDataFrame(operand1) )?operand1:operand2;

            /// 2.1. Предполагаем, что расчет не нужен
            unknownValue = value;

            /// 2.2. Заполнение байт данных фрейма предполагаемыми значениями
            if (opcode == OPCODE_LOG_OR)            frame.Data[byteFrameCount] = (value) ? 1 : 0;
            else if (opcode == OPCODE_LOG_AND)      frame.Data[byteFrameCount] = (value) ? 1 : 0;
            else if (opcode == OPCODE_BIT_OR)       frame.Data[byteFrameCount] = uint8_t(value);
            else if (opcode == OPCODE_BIT_XOR)      {/*TODO*/}
            else if (opcode == OPCODE_BIT_AND)      frame.Data[byteFrameCount] = uint8_t(value);
            else
                return 0;
        }
        else
            return 0;
    }
    else if (opcode == OPCODE_IF_ELSE)
    {
        if (operand1)
        {
            return operand2;
        }
        else
        {
            return 0xFFFFFFFF;
        }
    }
    return unknownValue;
}


/*
* @brief Определяет, является ли данный байт байтом данных фрейма
* @param byte - байт для рассмотрения
* @return 1 - является, 0 - нет
*/
uint8_t OBD::IsItDataFrame(const uint8_t byte)
{
    return (byte < 0x08) ? 1 : 0;
}


/*
* @brief Определяет, является ли данный байт константой
* @param byte - байт для рассмотрения
* @return 1 - является, 0 - нет
*/
uint8_t OBD::IsItConst(const uint8_t byte)
{
    return (byte >= 0x80) ? 1 : 0;
}


/*
* @brief Определяет, является ли данный байт оператором
* @param byte - байт для рассмотрения
* @return 1 - является, 0 - нет
*/
uint8_t OBD::IsItOperator(const uint8_t byte)
{
    return ((byte >= 0x08) && (byte < 0x80)) ? 1 : 0;
}


/*
* @brief Определяет, является ли данный байт операндом
* @param byte - байт для рассмотрения
* @return 1 - является, 0 - нет
*/
uint8_t OBD::IsItOperand(const uint8_t byte)
{
    return ((byte < 0x08) || (byte >= 0x80)) ? 1 : 0;
}


/*
* @brief Определяет, является ли данный байт унарным оператором
* @param byte - байт для рассмотрения
* @return 1 - является, 0 - нет
*/
uint8_t OBD::IsItUnaryOperator(const uint8_t byte)
{
    return ( (byte == OPCODE_LOG_NOT) || (byte == OPCODE_BIT_NOT) )? 1 : 0;
}


/*
* @brief Определяет, является ли данный байт бинарным оператором
* @param byte - байт для рассмотрения
* @return 1 - является, 0 - нет
*/
uint8_t OBD::IsItBinaryOperator(const uint8_t byte)
{
    return ( (byte == OPCODE_LOG_OR) || (byte == OPCODE_LOG_AND) || ( (byte >= OPCODE_BIT_OR) && (byte <= OPCODE_DIV) ) ) ? 1 : 0;
}


/*
* @brief Определяет, является ли данный байт тернарным оператором
* @param byte - байт для рассмотрения
* @return 1 - является, 0 - нет
*/
uint8_t OBD::IsItTernaryOperator(const uint8_t byte)
{
    return (byte == OPCODE_IF_ELSE) ? 1 : 0;
}
