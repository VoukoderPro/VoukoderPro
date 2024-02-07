#pragma once

#include <map>
#include <string>

namespace VoukoderPro
{
    typedef const std::string PropertyID;
    
    // Generic
    static PropertyID pPropApplication = "application";
    static PropertyID pPropApplicationVersion = "applicationversion";
    static PropertyID pPropFilename = "filename";
    static PropertyID pPropConfiguration = "configuration"; // obsolete

    // Track
    static PropertyID pPropId = "id";
    static PropertyID pPropType = "type";
    static PropertyID pPropWidth = "width";
    static PropertyID pPropHeight = "height";
    static PropertyID pPropTimebaseNum = "timebase_num";
    static PropertyID pPropTimebaseDen = "timebase_den";
    static PropertyID pPropAspectNum = "aspect_num";
    static PropertyID pPropAspectDen = "aspect_den";
    static PropertyID pPropTimecode = "timecode";
    static PropertyID pPropFieldOrder = "fieldorder";
    static PropertyID pPropOriginalFormat = "originalformat";
    static PropertyID pPropFormat = "format";
    static PropertyID pPropColorRange = "colorrange";
    static PropertyID pPropColorSpace = "colorspace";
    static PropertyID pPropColorPrimaries = "colorprimaries";
    static PropertyID pPropColorTransfer = "colortransfer";
    static PropertyID pPropSamplingRate = "samplingrate";
    static PropertyID pPropChannelLayout = "channellayout";
    static PropertyID pPropChannelCount = "channelcount";
    static PropertyID pPropLanguage = "language";
    static PropertyID pPropOutputUrl = "url";
    static PropertyID pPropChapters = "chapters";                   // Title;TimeBaseNum;TimeBaseDen;FrameStart[;FrameEnd][,...]

    //
    static PropertyID pVarFilePath = "FilePath";
    static PropertyID pVarFileName = "FileName";
    static PropertyID pVarFileExtension = "FileExtension";
    static PropertyID pVarFileFullname = "FileFullname";
}
