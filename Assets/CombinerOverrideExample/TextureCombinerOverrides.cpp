// File Format v1.0

// These enums and structs are verbatim copies of code within the Unity engine, so should not be modified.

enum Source
{
    None,                           // Can be used, for clarity, to indicate a combiner stage doesn't use a particular operand.
    PrimaryColor,
    // Note that lighting colours are not present, because lighting is calculated by Unity.
    Texture0,
    Texture1,
    Texture2,
    Texture3,
    PreviousBuffer,
    Constant,
    Previous
};

enum Operand
{
    SrcColor,                       // Not permitted for alpha combiners.
    OneMinusSrcColor,               // Not permitted for alpha combiners.
    SrcAlpha,
    OneMinusSrcAlpha,
    SrcRed,
    OneMinusSrcRed,
    SrcGreen,
    OneMinusSrcGreen,
    SrcBlue,
    OneMinusSrcBlue
};

enum Function
{
    Replace,
    Modulate,
    Add,
    AddSigned,
    Interpolate,
    Subtract,
    Dot3_RGB,                       // Not permitted for alpha combiners.
    Dot3_RGBA,                      // Not permitted for alpha combiners.
    MultAdd,
    AddMult
};

enum Scale
{
    One,
    Two,
    Four
};

enum Comparison
{
    Disable,
    Never,
    Less,
    Equal,
    LEQual,
    Greater,
    NotEqual,
    GEqual,
    Always
};

struct AlphaTesting
{
    int             comparison;
    const char*     propertyName;
    float           value;
};

struct Combiner
{
    unsigned char   source[3];      // Source
    unsigned char   operand[3];     // Operand (SrcColor and OneMinusSrcColor not permitted for alpha)
    unsigned char   function;       // Function (Dot3_RGB not permitted for alpha)
    unsigned char   scale;          // Scale
};

struct Stage
{
    Combiner        color;
    Combiner        alpha;
    const char*     textureName;
    const char*     propertyName;
    float           propertyValue[4];
};

struct TextureCombinerOverride
{
    // The name of the shader to override.
    const char*     shaderName;
    
    // Some shaders are instantiated multiple times with different combiner names.
    // If you need to override specific instances, you can specify its name here.
    const char*     combinerName;

    // How alpha-testing is performed.
    AlphaTesting    alphaTesting;

    // Up-to six combiner stages.
    Stage           stages[6];
};

/*
    In the following override, we'll add three textures together.

    There are a number of differences between Unity's texture combiners (which were designed to reflect texture
    combiners on other hardware) and the Nintendo 3DS's texture combiners.  Some of these differences require
    creative workarounds to make them compatible.  For example, Unity's texture combiner support only allows us
    to specifiy one texture per combiner stage, whereas the 3DS's texture combiner stages can address up to
    three textures in any of their combiner stages.
    (Technically four, but the fourth one is a procedural texture which is not supported by this system)

    Therefore, in order to coax all three textures through Unity's shader pipeline, it is neccessary to include
    three stages in the combiner, and specify one texture per stage.  To reiterate, we only have to do this in
    order to maintain the link between the textures and the shader.  It doesn't affect your ability to use any
    of the texture inputs in any of the combiner stages.
*/ 

// The name of this variable is significant; it must also be "static const".
// Only one override is provided in this example, but note that g_TextureCombinerOverrides is an array, so you can provide as many as you want.
static const TextureCombinerOverride g_TextureCombinerOverrides[] =
{
    {
        "AddThreeTextures",         // shaderName
        "",                         // combinerName
        { Greater, NULL, 0.0f },    // alphaTesting

        {
            // Stage 1.
            {
                // color
                Texture0, None, None,
                SrcColor, SrcColor, SrcColor,
                Replace,
                One,

                // alpha
                Texture0, None, None,
                SrcAlpha, SrcAlpha, SrcAlpha,
                Replace,
                One,

                // textureName.  This specifies "Texture0"
                "_TextureA",
            },

            // Stage 2.
            {
                // color
                Previous, Texture1, None,
                SrcColor, SrcColor, SrcColor,
                Add,
                One,

                // alpha
                Previous, None, None,
                SrcAlpha, SrcAlpha, SrcAlpha,
                Replace,
                One,

                // textureName.  This specifies "Texture1"
                "_TextureB",
            },

            // Stage 3.
            {
                // color
                Previous, Texture2, None,
                SrcColor, SrcColor, SrcColor,
                Add,
                One,

                // alpha
                Previous, None, None,
                SrcAlpha, SrcAlpha, SrcAlpha,
                Replace,
                One,

                // textureName.  This specifies "Texture2"
                "_TextureC",
            },

            // You can provide up to six stages, or else you must terminate the list like this.
            { None }
        },
    },
};
