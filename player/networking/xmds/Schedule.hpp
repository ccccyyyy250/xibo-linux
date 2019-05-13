#pragma once

#include "Soap.hpp"
#include "BaseRequestSerializer.hpp"
#include "BaseResponseParser.hpp"

#include "common/Field.hpp"

namespace Schedule
{
    struct Result
    {
        std::string scheduleXml;
    };

    struct Request
    {
        Field<std::string> serverKey{"serverKey"};
        Field<std::string> hardwareKey{"hardwareKey"};
    };
}

template<>
class Soap::RequestSerializer<Schedule::Request> : public BaseRequestSerializer<Schedule::Request>
{
public:
    RequestSerializer(const Schedule::Request& request);
    std::string string();

};

template<>
class Soap::ResponseParser<Schedule::Result> : public BaseResponseParser<Schedule::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    Schedule::Result doParse(const xml_node& scheduleNode) override;

};
