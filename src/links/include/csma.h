#ifndef __CSMA_H_INCLUDED__
#define __CSMA_H_INCLUDED__

#include <map>
#include <string>
#include <ostream>
#include <memory>

#include "yntdl.h"
#include "linkType.h"

class CSMA : public LinkType {
public:
    int getIfacesSupported() override;
    void writeIncludes(std::ostream& str) override;
    void writeTypeInit(std::ostream& str) override;
    void writeLinkInit(std::ostream& str, std::shared_ptr<yntdl::Link> linkPtr) override;
    void addIfacesToLink(std::ostream& str, std::shared_ptr<yntdl::Link> linkPtr) override;
    void writeClosingRemarks(std::ostream& str) override;
};

#endif