#include "obd.hpp"
#include "cstring"  // для memcpy
#include <iostream>

inline uint8_t IsItDataFrame(uint8_t byte)
{
    return (byte < 0x08)?1:0;
}

inline uint8_t IsItConst(uint8_t byte)
{
    return (byte >= 0x80)?1:0;
}

inline uint8_t IsItOperator(uint8_t byte)
{
    return ( (byte >= 0x08) && (byte < 0x80) )?1:0;
}

inline uint8_t IsItOperand(uint8_t byte)
{
    return ( (byte < 0x08) || (byte >= 0x80) )?1:0;
}


/*
* @brief Имитация инициализации тестового стенда
*/
void OBD::Init()
{
	ParamNumber = 1;
    Value = 0;

    uint8_t paramCount = 0;
    /*
    ParamTable[0].flength = 32;
    uint8_t arr[32] = {0x01, 0x1B, 0x1B, 0x18, 0x1A, 0x18, 0x1A, 0x19,
                       0x80, 0x82, 0x17, 0x03, 0x87, 0x81, 0x0D, 0x16,
                       0x0F, 0x03, 0xFF, 0x88, 0x02, 0x1A, 0x1A, 0x1A,
                       0x17, 0x03, 0x87, 0xA0, 0xA0, 0xA0, 0xA8, 0x99};
    */
    ParamTable[paramCount].FormulaLength = 12;
    uint8_t arr[12] = { 0x01, 0x1A, 0x0D, 0x0D, 0x16, 0x02, 0x90, 0x16, 0x01, 0x88, 0x00, 0x8A};
    memcpy(ParamTable[paramCount].Formula, arr, ParamTable[paramCount].FormulaLength);
	CreateTrees();
}


/*
* @brief Построение дерева синтаксического разбора формулы
*/
void OBD::CreateTrees()
{
	for (uint8_t paramCount = 0; paramCount < ParamNumber; paramCount++)
	{
		for (uint8_t count = ParamTable[paramCount].FormulaLength; count >= 1;)
		{
			uint8_t byte = ParamTable[paramCount].Formula[--count];
			// Если операнд
			if ((byte < 0x08) || (byte >= 0x80))
			{
				Tree::Node* op = ParamTable[paramCount].tree.AddNodeLower(byte);
				buf.Push(op);
			}
			// Если оператор
			else
			{
				// Если унарный оператор <=> 1 наследник
				if ((byte == OPCODE_LOG_NOT) || (byte == OPCODE_BIT_NOT))
				{
					uint8_t length = 1;
					Tree::Node** ops = new Tree::Node*[length];
					ops[0] = buf.Pop();
					Tree::Node* op = ParamTable[paramCount].tree.AddNodeParent(byte, ops, length);
					buf.Push(op);
				}
				// Если бинарный оператор <=> 2 наследника
				else if ((byte == OPCODE_LOG_OR) || (byte == OPCODE_LOG_AND) || ((byte >= OPCODE_BIT_OR) && (byte <= OPCODE_DIV)))
				{
					uint8_t length = 2;
					Tree::Node** ops = new Tree::Node*[length];
					ops[0] = buf.Pop();
					ops[1] = buf.Pop();
					Tree::Node* op = ParamTable[paramCount].tree.AddNodeParent(byte, ops, length);
					buf.Push(op);
				}
				// Если тернарный оператор <=> 3 наследника
				else if (byte == OPCODE_IF_ELSE)
				{
					uint8_t length = 3;
					Tree::Node** ops = new Tree::Node*[length];
					ops[0] = buf.Pop();
					ops[1] = buf.Pop();
					ops[2] = buf.Pop();
					Tree::Node* op = ParamTable[paramCount].tree.AddNodeParent(byte, ops, length);
					buf.Push(op);
				}

			}
		}
	}
}


/*
* @brief Выполнение прямого расчета по формуле
* @note Скопировано из тестового стенда
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

        /// Îïåðàíä (èíäåêñ ÿ÷åéêè êàäðà)
        if (byte < 8)
        {
            operands[++operandIndex] = frame.Data[byte];
        }
        /// Îïåðàòîð
        else if ((byte >= 8) && (byte <= 127))
        {
            operators[++operatorIndex] = byte;
        }
        /// Îïåðàíä (êîíñòàíòà)
        else
        {
            operands[++operandIndex] = (byte & 0x7F);
        }

        /// Ïðîâåðèòü, ìîæíî ëè âûïîëíèòü îïåðàöèþ
        if ((operatorIndex >= 0) && (operandIndex >= 1))
        {
            /// Âûáðàòü ÷èñëî îïåðàíäîâ, â çàâèñèìîñòè îò îïåðàòîðà
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
* @return рассчитанное число
*/
uint32_t OBD::DoReverseCalculateWithTree(uint32_t value, Tree::Node* node)
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

    // 0. Check correctness:
    if (node == nullptr)
    {
        if (base == nullptr)
            return BASE_IS_NULL;
        node = base;
        status = OK;
    }
    if (status != OK)
        return 0;

    // +1. If this node is OPERAND => this node is apex (висячая вершина)
    if (node->ChildsCount == 0)
    {
        // This node must be Base
        if ((node == base) && IsItDataFrame(node->Value) )
        {
            frame.Data[node->Value] = value;
        }
        // Else, there is error in formula
        else
            status = FORMULA_ERROR_NODE_IS_OPERAND;
    }
    // -2. If this node is UNARY OPERATOR, try to calculate it
    else if (node->ChildsCount == 1)
    {
        uint8_t childByte = base->ChildsArr[0]->Value;

        // If child node is OPERAND, try to calculate it
        if (IsItOperand(childByte))
        {
            value = CalculateReverseElementary(value, node->Value, childByte);
        }
        // If child node is OPERATOR, try to recursively calculate it
        else
        {
            uint8_t status = DoReverseCalculateWithTree(value, node->ChildsArr[0]);
            if (status != OK)
                status = UNEXPECTED_ERROR;
            /*TODO: обработка других ошибок*/
        }
    }
    // -3. If this node is BINARY OPERATOR, try to calculate it
    else if (node->ChildsCount == 2)
    {
        uint8_t operand1 = node->ChildsArr[0]->Value;
        uint8_t operand2 = node->ChildsArr[1]->Value;

        // Оба потомка - константы
        if ( IsItConst(operand1) && IsItConst(operand2) )
            value = CalculateDirectElementary(node->Value, operand1, operand2);

        // Хотя бы один из потомков - операнд
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
    // -4. If this node is TERNARY OPERATOR, try to calculate it
    else if (node->ChildsCount == 3)
    {
        /*На практике таких формул не было замечено*/
    }
    // +5. Если текущий узел - не пойми что
    else
        status = UNEXPECTED_ERROR;

    return value;
}


/*
* @brief Обратный расчет с помощью метода перебора
* @note Может выполняться очень долго (до получаса при 4 байтах)
* @param NeedValue - число, которое должен распарсить терминал
* @return статус выполнения: 0 - все хорошо, иначе ошибка
*/
uint8_t OBD::DoReverseCalculateWithBruteForce(int64_t needValue)
{
    enum : uint8_t
    {
        OK = 0,
        ERROR = 1,
    };
    for (uint16_t d0 = 0; d0 < 256; d0++)
    {
        for (uint16_t d1 = 0; d1 < 256; d1++)
        {
            for (uint16_t d2 = 0; d2 < 256; d2++)
            {
                frame.Data[0] = (uint8_t)d0; frame.Data[1] = (uint8_t)d1; frame.Data[2] = (uint8_t)d2;
                DoDirectCalculate();
                if (Value == needValue)
                {
                    return OK;
                }
            }
        }
    }
    return ERROR;
}


/*
* @brief Обратный расчет с помощью метода дихотомии
* @note Может не сойтись и зависнуть, однако относительно быстрый
* @param NeedValue - число, которое должен распарсить терминал
* @return статус выполнения: 0 - все хорошо, иначе ошибка
*/
uint8_t OBD::DoReverseCalculateWithMethodDichotomy(int64_t NeedValue)
{
    enum : uint8_t
    {
        OK = 0,
        ERROR = 1,
    };
    uint32_t dataMinBorder = 0;
    uint32_t dataMaxBorder = 16777215;
    uint32_t dataGuess = 8388608;
    frame.Data[0] = dataGuess % 256;
    frame.Data[1] = (dataGuess >> 8) % 256;
    frame.Data[2] = (dataGuess >> 16) % 256;
    DoDirectCalculate();
    while (Value != NeedValue)
    {
        int64_t dif = Value - NeedValue;
        // Если нужное число слева
        if (dif > 0)
        {
            dataMaxBorder = dataGuess;
            dataGuess = (dataMaxBorder + dataMinBorder) >> 1;	// округляем в меньшую сторону
        }
        else
        {
            dataMinBorder = dataGuess;
            dataGuess = ((dataMaxBorder + dataMinBorder) >> 1) + 1;	// округляем в большую сторону
        }
        frame.Data[0] = dataGuess % 256;
        frame.Data[1] = (dataGuess >> 8) % 256;
        frame.Data[2] = (dataGuess >> 16) % 256;
        DoDirectCalculate();
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
uint32_t OBD::CalculateDirectElementary(uint8_t opcode, uint32_t operand1, uint32_t operand2, uint32_t operand3)
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
uint32_t OBD::CalculateReverseElementary(uint32_t value, uint8_t opcode, uint32_t operand1, uint32_t operand2, uint32_t operand3)
{
    uint32_t unknownValue = 0;
    // (+-)1. Если унарный оператор
    if ( (opcode == OPCODE_LOG_NOT) || (opcode == OPCODE_BIT_NOT) )
        unknownValue = CalculateDirectElementary(opcode, value);

    // (+-)2. Если бинарный оператор
    else if ( (opcode == OPCODE_LOG_OR) || (opcode == OPCODE_LOG_AND) || ( (opcode >= OPCODE_BIT_OR)&&(opcode <= OPCODE_DIV) ) )
    {
        // 2.1. Выполнение обратного элементарного расчета, если есть хотя бы одна константа
        if ( IsItConst(operand1) || IsItConst(operand2) )
        {
            uint8_t knownConstant = ( IsItConst(operand1) )?operand1:operand2;

            // 2.1.1. Выполнение расчета
            if (opcode == OPCODE_LOG_OR)            unknownValue = (value) ? 1 : 0;
            else if (opcode == OPCODE_LOG_AND)      unknownValue = (value) ? 1 : 0;
            else if (opcode == OPCODE_BIT_OR)       unknownValue = (value&(~knownConstant));
            else if (opcode == OPCODE_BIT_XOR)      {/*TODO*/}
            else if (opcode == OPCODE_BIT_AND)      unknownValue = (value&knownConstant);
            else if (opcode == OPCODE_EQU)          unknownValue = (value) ? knownConstant : 0;
            else if (opcode == OPCODE_NEQU)         unknownValue = (value) ? 0 : knownConstant;
            else if (opcode == OPCODE_LESS)         {/*TODO*/}
            else if (opcode == OPCODE_MORE)         {/*TODO*/}
            else if (opcode == OPCODE_LESS_EQU)     {/*TODO*/}
            else if (opcode == OPCODE_MORE_EQU)     {/*TODO*/}
            else if (opcode == OPCODE_SHIFT_LEFT)   unknownValue = value >> knownConstant;
            else if (opcode == OPCODE_SHIFT_RIGHT)  unknownValue = value << knownConstant;
            else if (opcode == OPCODE_ADD)          unknownValue = value - (knownConstant - 0x80);
            else if (opcode == OPCODE_SUB)          // dependent
            {
                if( IsItConst(operand2) )           unknownValue = value + (knownConstant - 0x80);
                else                                unknownValue = (knownConstant - 0x80) - value;
            }
            else if (opcode == OPCODE_MUL)          unknownValue = value / (knownConstant - 0x80);
            else if (opcode == OPCODE_DIV)          // dependent
            {
                if( IsItConst(operand2) )           unknownValue = value * (knownConstant - 0x80);
                else                                unknownValue = (knownConstant - 0x80) / value;
            }
            else
                return 0;

            // 2.1.2. Заполнение байт данных фрейма соответствующими значениями
            if ( IsItDataFrame(operand1) )
                frame.Data[operand1] |= unknownValue;
            else if ( IsItDataFrame(operand2) )
                frame.Data[operand2] |= unknownValue;
        }

        // 2.2. Попытка выполнения обратного элементарного расчета, если константы нет
        else if ( IsItDataFrame(operand1) || IsItDataFrame(operand2) )
        {
            uint8_t byteFrameCount = ( IsItDataFrame(operand1) )?operand1:operand2;

            // 2.2.1. Предполагаем, что расчет не нужен
            unknownValue = value;

            // 2.2.2. Заполнение байт данных фрейма предполагаемыми значениями
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
    // 3. Если тернарный оператор
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
* @brief Выводит в терминал формулу, полученную из конфигуратора
*/
void OBD::ShowFormula()
{
    std::cout << "\nFormula[" << ParamCount + 0 << "]: " << std::endl;
    std::cout << "Length = " << ParamTable[ParamCount].FormulaLength + 0 << std::endl;
    for(uint8_t countOfByte = 1; countOfByte < ParamTable[ParamCount].FormulaLength; countOfByte++)
    {
        uint8_t byte = ParamTable[ParamCount].Formula[countOfByte];
        ShowByte(byte);
    }
}


/*
* @brief Выводит в терминал распарсенный байт формулы
*/
void OBD::ShowByte(uint8_t byte)
{
    if(byte < 0x08)
        std::cout << "d" << byte + 0 << " ";
    else if(byte >= 0x80)
        std::cout << byte - 0x80 << " ";
    else if(byte == OPCODE_ADD)
        std::cout << "+ ";
    else if(byte == OPCODE_SUB)
        std::cout << "- ";
    else if(byte == OPCODE_DIV)
        std::cout << "/ ";
    else if(byte == OPCODE_MUL)
        std::cout << "* ";
    else if(byte == OPCODE_BIT_AND)
        std::cout << "& ";
    else if(byte == OPCODE_BIT_OR)
        std::cout << "| ";
    else if(byte == OPCODE_SHIFT_LEFT)
        std::cout << "<< ";
    else if(byte == OPCODE_SHIFT_RIGHT)
        std::cout << ">> ";
    else
        std::cout << "? ";
}


/*
* @brief Выводит в терминал операторы дерева, начиная с верхнего узла, если возможно
*/
void OBD::ShowTree()
{
    Tree::Node* ptrNode = ParamTable[ParamCount].tree.GetBaseNode();
    std::cout << std::endl << "Tree:" << std::endl;
    if (ptrNode != nullptr)
    {
        ShowTreeNode(ptrNode);
    }
    else
        std::cout <<std::endl << "Tree is empty";
}


/*
* @brief Рекурсивно выводит в терминал операторы дерева
*/
void OBD::ShowTreeNode(Tree::Node* ptrNode)
{
    if (ptrNode != nullptr)
    {
        if ( IsItOperator(ptrNode->Value) )
        {
            ShowByte(ptrNode->Value);
            ShowTreeNode(ptrNode->ChildsArr[0]);
            ShowTreeNode(ptrNode->ChildsArr[1]);
        }
        else
            std::cout << "   ";
    }
}
