// PP JOIN Implementation - Concatenate severals macro
#define PP_JOIN_IMPL(_0, _1)		_0##_1
#define PP_JOIN(_0, _1)				PP_JOIN_IMPL(_0, _1)
#define PP_JOIN_2(_0, _1)			PP_JOIN(_0, _1)
#define PP_JOIN_3(_0, _1, _2)		PP_JOIN_2(PP_JOIN_2(_0, _1), _2)

// PP COMA - Handle the coma if necessary
#define PP_COMA(...) PP_MACRO(PP_COMA, __VA_ARGS__)
#define PP_COMA_0 
#define PP_COMA_1 ,
#define PP_COMA_2 ,
#define PP_COMA_3 ,
#define PP_COMA_4 ,
#define PP_COMA_5 ,
#define PP_COMA_6 ,
#define PP_COMA_7 ,
#define PP_COMA_8 ,
#define PP_COMA_9 ,
#define PP_COMA_10 ,

// PP TYPE - Declare a type depenging on the numer of arguments
#define PP_TYPE(_name_, ...) PP_MACRO_CALL(PP_TYPE, _name_, __VA_ARGS__)
#define PP_TYPE_1(_name_) _name_ ## _0
#define PP_TYPE_2(_name_, _type1_)_name_ ## _1<_type1_>
#define PP_TYPE_3(_name_, _type1_, _type2_) _name_ ## _2<_type1_, _type2_>
#define PP_TYPE_4(_name_, _type1_, _type2_, _type3_) _name_ ## _3<_type1_, _type2_, _type3_>
#define PP_TYPE_5(_name_, _type1_, _type2_, _type3_, _type4_) _name_ ## _4<_type1_, _type2_, _type3_, _type4_>
#define PP_TYPE_6(_name_, _type1_, _type2_, _type3_, _type4_, _type5_) _name_ ## _5<_type1_, _type2_, _type3_, _type4_, _type5_>
#define PP_TYPE_7(_name_, _type1_, _type2_, _type3_, _type4_, _type5_, _type6_) _name_ ## _6<_type1_, _type2_, _type3_, _type4_, _type5_, _type6_>
#define PP_TYPE_8(_name_, _type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_) _name_ ## _7<_type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_>
#define PP_TYPE_9(_name_, _type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_, _type8_) _name_ ## _8<_type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_, _type8_>
#define PP_TYPE_10(_name_, _type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_, _type8_, _type9_) _name_ ## _9<_type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_, _type8_, _type9_>
#define PP_TYPE_11(_name_, _type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_, _type8_, _type9_, _type10_) _name_ ## _10<_type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_, _type8_, _type9_, _type10_>

// PP NARG Implementation - Get a result depending on the number of arguments
#define PP_VA_ARGS(...) (__VA_ARGS__)
#define PP_NARG_IMPL(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N 
#define PP_COUNT_ARGS(...) PP_JOIN_2(PP_NARG_IMPL, (-1, __VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define PP_MACRO(_name_, ...) PP_JOIN_2(_name_ ## _, PP_COUNT_ARGS(__VA_ARGS__))
#define PP_MACRO_CALL(_name_, ...) PP_JOIN_3(_name_ ## _, PP_COUNT_ARGS(__VA_ARGS__), PP_VA_ARGS(__VA_ARGS__))

// PP CLASS - Declare a class
#define PP_CLASS(_name_, ...) PP_MACRO_CALL(PP_CLASS, _name_, __VA_ARGS__)
#define PP_CLASS_1(_name_) class _name_ ## _0
#define PP_CLASS_2(_name_, _type1_) template <typename _type1_> class _name_ ## _1
#define PP_CLASS_3(_name_, _type1_, _type2_) template <typename _type1_, typename _type2_> class _name_ ## _2
#define PP_CLASS_4(_name_, _type1_, _type2_, _type3_) template <typename _type1_, typename _type2_, typename _type3_> class _name_ ## _3
#define PP_CLASS_5(_name_, _type1_, _type2_, _type3_, _type4_) template <typename _type1_, typename _type2_, typename _type3_, typename _type4_> class _name_ ## _4
#define PP_CLASS_6(_name_, _type1_, _type2_, _type3_, _type4_, _type5_) template <typename _type1_, typename _type2_, typename _type3_, typename _type4_, typename _type5_> class _name_ ## _5
#define PP_CLASS_7(_name_, _type1_, _type2_, _type3_, _type4_, _type5_, _type6_) template <typename _type1_, typename _type2_, typename _type3_, typename _type4_, typename _type5_, typename _type6_> class _name_ ## _6
#define PP_CLASS_8(_name_, _type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_) template <typename _type1_, typename _type2_, typename _type3_, typename _type4_, typename _type5_, typename _type6_, typename _type7_> class _name_ ## _7
#define PP_CLASS_9(_name_, _type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_, _type8_) template <typename _type1_, typename _type2_, typename _type3_, typename _type4_, typename _type5_, typename _type6_, typename _type7_, typename _type8_> class _name_ ## _8
#define PP_CLASS_10(_name_, _type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_, _type8_, _type9_) template <typename _type1_, typename _type2_, typename _type3_, typename _type4_, typename _type5_, typename _type6_, typename _type7_, typename _type8_, typename _type9_> class _name_ ## _9
#define PP_CLASS_11(_name_, _type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_, _type8_, _type9_, _type10_) template <typename _type1_, typename _type2_, typename _type3_, typename _type4_, typename _type5_, typename _type6_, typename _type7_, typename _type8_, typename _type9_, typename _type10_> class _name_ ## _10

// PP FUNCTION ARGS - Declare argument in prototype function
#define PP_FUNCTION_ARGS(...) PP_MACRO_CALL(PP_FUNCTION_ARGS, __VA_ARGS__)
#define PP_FUNCTION_ARGS_0()
#define PP_FUNCTION_ARGS_1(_type1_) _type1_ arg1
#define PP_FUNCTION_ARGS_2(_type1_, _type2_) _type1_ arg1, _type2_ arg2
#define PP_FUNCTION_ARGS_3(_type1_, _type2_, _type3_) _type1_ arg1, _type2_ arg2, _type3_ arg3
#define PP_FUNCTION_ARGS_4(_type1_, _type2_, _type3_, _type4_) _type1_ arg1, _type2_ arg2, _type3_ arg3, _type4_ arg4
#define PP_FUNCTION_ARGS_5(_type1_, _type2_, _type3_, _type4_, _type5_) _type1_ arg1, _type2_ arg2, _type3_ arg3, _type4_ arg4, _type5_ arg5
#define PP_FUNCTION_ARGS_6(_type1_, _type2_, _type3_, _type4_, _type5_, _type6_) _type1_ arg1, _type2_ arg2, _type3_ arg3, _type4_ arg4, _type5_ arg5, _type6_ arg6
#define PP_FUNCTION_ARGS_7(_type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_) _type1_ arg1, _type2_ arg2, _type3_ arg3, _type4_ arg4, _type5_ arg5, _type6_ arg6, _type7_ arg7
#define PP_FUNCTION_ARGS_8(_type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_, _type8_) _type1_ arg1, _type2_ arg2, _type3_ arg3, _type4_ arg4, _type5_ arg5, _type6_ arg6, _type7_ arg7, _type8_ arg8
#define PP_FUNCTION_ARGS_9(_type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_, _type8_, _type9_) _type1_ arg1, _type2_ arg2, _type3_ arg3, _type4_ arg4, _type5_ arg5, _type6_ arg6, _type7_ arg7, _type8_ arg8, _type9_ arg9
#define PP_FUNCTION_ARGS_10(_type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_, _type8_, _type9_, _type10_) _type1_ arg1, _type2_ arg2, _type3_ arg3, _type4_ arg4, _type5_ arg5, _type6_ arg6, _type7_ arg7, _type8_ arg8, _type9_ arg9, _type10_ arg10

// PP FUNCTION VALUE - Declare argument value (linked to PP_FUNCTION_ARGS)
#define PP_FUNCTION_VALUE(...) PP_MACRO_CALL(PP_FUNCTION_VALUE, __VA_ARGS__)
#define PP_FUNCTION_VALUE_0()
#define PP_FUNCTION_VALUE_1(_type1_) arg1
#define PP_FUNCTION_VALUE_2(_type1_, _type2_) arg1, arg2
#define PP_FUNCTION_VALUE_3(_type1_, _type2_, _type3_) arg1, arg2, arg3
#define PP_FUNCTION_VALUE_4(_type1_, _type2_, _type3_, _type4_) arg1, arg2, arg3, arg4
#define PP_FUNCTION_VALUE_5(_type1_, _type2_, _type3_, _type4_, _type5_) arg1, arg2, arg3, arg4, arg5
#define PP_FUNCTION_VALUE_6(_type1_, _type2_, _type3_, _type4_, _type5_, _type6_) arg1, arg2, arg3, arg4, arg5, arg6
#define PP_FUNCTION_VALUE_7(_type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_) arg1, arg2, arg3, arg4, arg5, arg6, arg7
#define PP_FUNCTION_VALUE_8(_type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_, _type8_) arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8
#define PP_FUNCTION_VALUE_9(_type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_, _type8_, _type9_) arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9
#define PP_FUNCTION_VALUE_10(_type1_, _type2_, _type3_, _type4_, _type5_, _type6_, _type7_, _type8_, _type9_, _type10_) arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10