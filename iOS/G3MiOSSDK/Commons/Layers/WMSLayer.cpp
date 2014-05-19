//
//  WMSLayer.cpp
//  G3MiOSSDK
//
//  Created by José Miguel S N on 18/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "WMSLayer.hpp"

#include "LayerTilesRenderParameters.hpp"
#include "MercatorUtils.hpp"
#include "Tile.hpp"
#include "Petition.hpp"
#include "LayerCondition.hpp"
#include "RenderState.hpp"
#include "TimeInterval.hpp"

WMSLayer::WMSLayer(const std::string&                mapLayer,
                   const URL&                        mapServerURL,
                   const WMSServerVersion            mapServerVersion,
                   const std::string&                queryLayer,
                   const URL&                        queryServerURL,
                   const WMSServerVersion            queryServerVersion,
                   const Sector&                     dataSector,
                   const std::string&                format,
                   const std::string&                srs,
                   const std::string&                style,
                   const bool                        isTransparent,
                   const LayerCondition*             condition,
                   const TimeInterval&               timeToCache,
                   const bool                        readExpired,
                   const LayerTilesRenderParameters* parameters,
                   const float                       transparency,
                   const std::string&                disclaimerInfo):
RasterLayer(timeToCache,
            readExpired,
            (parameters == NULL)
            ? LayerTilesRenderParameters::createDefaultWGS84(Sector::fullSphere(), 0, 17)
            : parameters,
            transparency,
            condition,
            disclaimerInfo),
_mapLayer(mapLayer),
_mapServerURL(mapServerURL),
_mapServerVersion(mapServerVersion),
_dataSector(dataSector),
_queryLayer(queryLayer),
_queryServerURL(queryServerURL),
_queryServerVersion(queryServerVersion),
_format(format),
_srs(srs),
_style(style),
_isTransparent(isTransparent),
_extraParameter("")
{
}

WMSLayer::WMSLayer(const std::string&                mapLayer,
                   const URL&                        mapServerURL,
                   const WMSServerVersion            mapServerVersion,
                   const Sector&                     dataSector,
                   const std::string&                format,
                   const std::string&                srs,
                   const std::string&                style,
                   const bool                        isTransparent,
                   const LayerCondition*             condition,
                   const TimeInterval&               timeToCache,
                   const bool                        readExpired,
                   const LayerTilesRenderParameters* parameters,
                   const float                       transparency,
                   const std::string&                disclaimerInfo):
RasterLayer(timeToCache,
            readExpired,
            (parameters == NULL)
            ? LayerTilesRenderParameters::createDefaultWGS84(Sector::fullSphere(), 0, 17)
            : parameters,
            transparency,
            condition,
            disclaimerInfo),
_mapLayer(mapLayer),
_mapServerURL(mapServerURL),
_mapServerVersion(mapServerVersion),
_dataSector(dataSector),
_queryLayer(mapLayer),
_queryServerURL(mapServerURL),
_queryServerVersion(mapServerVersion),
_format(format),
_srs(srs),
_style(style),
_isTransparent(isTransparent),
_extraParameter("")
{
    
}

double WMSLayer::toBBOXLongitude(const Angle& longitude) const {
    return (_parameters->_mercator) ? MercatorUtils::longitudeToMeters(longitude) : longitude._degrees;
}

double WMSLayer::toBBOXLatitude(const Angle& latitude) const {
    return (_parameters->_mercator) ? MercatorUtils::latitudeToMeters(latitude) : latitude._degrees;
}

std::vector<Petition*> WMSLayer::createTileMapPetitions(const G3MRenderContext* rc,
                                                        const LayerTilesRenderParameters* layerTilesRenderParameters,
                                                        const Tile* tile) const {
    std::vector<Petition*> petitions;
    
    const std::string path = _mapServerURL._path;
    if (path.empty()) {
        return petitions;
    }
    
    const Sector tileSector = tile->_sector;
    if (!_dataSector.touchesWith(tileSector)) {
        return petitions;
    }
    
    const Sector sector = tileSector.intersection(_dataSector);
    if (sector._deltaLatitude.isZero() ||
        sector._deltaLongitude.isZero() ) {
        return petitions;
    }
    
    //TODO: MUST SCALE WIDTH,HEIGHT
    
    const Vector2I tileTextureResolution = _parameters->_tileTextureResolution;
    
	//Server name
    std::string req = path;
	if (req[req.size() - 1] != '?') {
		req += '?';
	}
    
    //  //If the server refer to itself as localhost...
    //  const int localhostPos = req.find("localhost");
    //  if (localhostPos != -1) {
    //    req = req.substr(localhostPos+9);
    //
    //    const int slashPos = req.find("/", 8);
    //    std::string newHost = req.substr(0, slashPos);
    //
    //    req = newHost + req;
    //  }
    
    req += "REQUEST=GetMap&SERVICE=WMS";
    
    
    switch (_mapServerVersion) {
        case WMS_1_3_0:
        {
            req += "&VERSION=1.3.0";
            
            IStringBuilder* isb = IStringBuilder::newStringBuilder();
            
            isb->addString("&WIDTH=");
            isb->addInt(tileTextureResolution._x);
            isb->addString("&HEIGHT=");
            isb->addInt(tileTextureResolution._y);
            
            isb->addString("&BBOX=");
            isb->addDouble( toBBOXLatitude( sector._lower._latitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLongitude( sector._lower._longitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLatitude( sector._upper._latitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLongitude( sector._upper._longitude ) );
            
            req += isb->getString();
            delete isb;
            
            req += "&CRS=EPSG:4326";
            
            break;
        }
        case WMS_1_1_0:
        default:
        {
            // default is 1.1.1
            req += "&VERSION=1.1.1";
            
            IStringBuilder* isb = IStringBuilder::newStringBuilder();
            
            isb->addString("&WIDTH=");
            isb->addInt(tileTextureResolution._x);
            isb->addString("&HEIGHT=");
            isb->addInt(tileTextureResolution._y);
            
            isb->addString("&BBOX=");
            isb->addDouble( toBBOXLongitude( sector._lower._longitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLatitude( sector._lower._latitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLongitude( sector._upper._longitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLatitude( sector._upper._latitude ) );
            
            req += isb->getString();
            delete isb;
            break;
        }
    }
    
    req += "&LAYERS=" + _mapLayer;
    
	req += "&FORMAT=" + _format;
    
    if (_srs != "") {
        req += "&SRS=" + _srs;
    }
	else {
        req += "&SRS=EPSG:4326";
    }
    
    //Style
    if (_style != "") {
        req += "&STYLES=" + _style;
    }
	else {
        req += "&STYLES=";
    }
    
    //ASKING TRANSPARENCY
    if (_isTransparent) {
        req += "&TRANSPARENT=TRUE";
    }
    else {
        req += "&TRANSPARENT=FALSE";
    }
    
    if (_extraParameter.compare("") != 0) {
        req += "&";
        req += _extraParameter;
    }
    
    //  printf("Request: %s\n", req.c_str());
    
    Petition *petition = new Petition(sector,
                                      URL(req, false),
                                      getTimeToCache(),
                                      getReadExpired(),
                                      _isTransparent,
                                      _transparency);
    petitions.push_back(petition);
    
	return petitions;
}

const URL WMSLayer::createURL(const Tile* tile) const {
    
    const std::string path = _mapServerURL._path;
    //  if (path.empty()) {
    //    return petitions;
    //  }
    
    const Sector tileSector = tile->_sector;
    //  if (!_sector.touchesWith(tileSector)) {
    //    return petitions;
    //  }
    //
    const Sector sector = tileSector.intersection(_dataSector);
    //  if (sector._deltaLatitude.isZero() ||
    //      sector._deltaLongitude.isZero() ) {
    //    return petitions;
    //  }
    
    //TODO: MUST SCALE WIDTH,HEIGHT
    
    const Vector2I tileTextureResolution = _parameters->_tileTextureResolution;
    
	//Server name
    std::string req = path;
	if (req[req.size() - 1] != '?') {
		req += '?';
	}
    
    //  //If the server refer to itself as localhost...
    //  const int localhostPos = req.find("localhost");
    //  if (localhostPos != -1) {
    //    req = req.substr(localhostPos+9);
    //
    //    const int slashPos = req.find("/", 8);
    //    std::string newHost = req.substr(0, slashPos);
    //
    //    req = newHost + req;
    //  }
    
    req += "REQUEST=GetMap&SERVICE=WMS";
    
    
    switch (_mapServerVersion) {
        case WMS_1_3_0:
        {
            req += "&VERSION=1.3.0";
            
            IStringBuilder* isb = IStringBuilder::newStringBuilder();
            
            isb->addString("&WIDTH=");
            isb->addInt(tileTextureResolution._x);
            isb->addString("&HEIGHT=");
            isb->addInt(tileTextureResolution._y);
            
            isb->addString("&BBOX=");
            isb->addDouble( toBBOXLatitude( sector._lower._latitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLongitude( sector._lower._longitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLatitude( sector._upper._latitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLongitude( sector._upper._longitude ) );
            
            req += isb->getString();
            delete isb;
            
            req += "&CRS=EPSG:4326";
            
            break;
        }
        case WMS_1_1_0:
        default:
        {
            // default is 1.1.1
            req += "&VERSION=1.1.1";
            
            IStringBuilder* isb = IStringBuilder::newStringBuilder();
            
            isb->addString("&WIDTH=");
            isb->addInt(tileTextureResolution._x);
            isb->addString("&HEIGHT=");
            isb->addInt(tileTextureResolution._y);
            
            isb->addString("&BBOX=");
            isb->addDouble( toBBOXLongitude( sector._lower._longitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLatitude( sector._lower._latitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLongitude( sector._upper._longitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLatitude( sector._upper._latitude ) );
            
            req += isb->getString();
            delete isb;
            break;
        }
    }
    
    req += "&LAYERS=" + _mapLayer;
    
	req += "&FORMAT=" + _format;
    
    if (_srs != "") {
        req += "&SRS=" + _srs;
    }
	else {
        req += "&SRS=EPSG:4326";
    }
    
    //Style
    if (_style != "") {
        req += "&STYLES=" + _style;
    }
	else {
        req += "&STYLES=";
    }
    
    //ASKING TRANSPARENCY
    if (_isTransparent) {
        req += "&TRANSPARENT=TRUE";
    }
    else {
        req += "&TRANSPARENT=FALSE";
    }
    
    if (_extraParameter.compare("") != 0) {
        req += "&";
        req += _extraParameter;
    }
    
    return URL(req, false);
}

URL WMSLayer::getFeatureInfoURL(const Geodetic2D& position,
                                const Sector& tileSector) const {
    if (!_dataSector.touchesWith(tileSector)) {
        return URL::nullURL();
    }
    
    const Sector intersectionSector = tileSector.intersection(_dataSector);
    
	//Server name
    std::string req = _queryServerURL._path;
	if (req[req.size()-1] != '?') {
		req += '?';
	}
    
    //If the server refer to itself as localhost...
    int pos = req.find("localhost");
    if (pos != -1) {
        req = req.substr(pos+9);
        
        int pos2 = req.find("/", 8);
        std::string newHost = req.substr(0, pos2);
        
        req = newHost + req;
    }
    
    req += "REQUEST=GetFeatureInfo&SERVICE=WMS";
    
    //SRS
    if (_srs != "") {
        req += "&SRS=" + _srs;
    }
	else {
        req += "&SRS=EPSG:4326";
    }
    
    switch (_queryServerVersion) {
        case WMS_1_3_0:
        {
            req += "&VERSION=1.3.0";
            
            IStringBuilder* isb = IStringBuilder::newStringBuilder();
            
            isb->addString("&WIDTH=");
            isb->addInt(_parameters->_tileTextureResolution._x);
            isb->addString("&HEIGHT=");
            isb->addInt(_parameters->_tileTextureResolution._y);
            
            isb->addString("&BBOX=");
            isb->addDouble( toBBOXLatitude( intersectionSector._lower._latitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLongitude( intersectionSector._lower._longitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLatitude( intersectionSector._upper._latitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLongitude( intersectionSector._upper._longitude ) );
            
            req += isb->getString();
            
            delete isb;
            
            req += "&CRS=EPSG:4326";
            
            break;
        }
        case WMS_1_1_0:
        default:
        {
            // default is 1.1.1
            req += "&VERSION=1.1.1";
            
            IStringBuilder* isb = IStringBuilder::newStringBuilder();
            
            isb->addString("&WIDTH=");
            isb->addInt(_parameters->_tileTextureResolution._x);
            isb->addString("&HEIGHT=");
            isb->addInt(_parameters->_tileTextureResolution._y);
            
            isb->addString("&BBOX=");
            isb->addDouble( toBBOXLongitude( intersectionSector._lower._longitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLatitude( intersectionSector._lower._latitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLongitude( intersectionSector._upper._longitude ) );
            isb->addString(",");
            isb->addDouble( toBBOXLatitude( intersectionSector._upper._latitude ) );
            
            req += isb->getString();
            
            delete isb;
            break;
        }
    }
    req += "&LAYERS=" + _queryLayer;
    req += "&QUERY_LAYERS=" + _queryLayer;
    
    req += "&INFO_FORMAT=text/plain";
    
    const IMathUtils* mu = IMathUtils::instance();
    
    double u;
    double v;
    if (_parameters->_mercator) {
        u = intersectionSector.getUCoordinate(position._longitude);
        v = MercatorUtils::getMercatorV(position._latitude);
    }
    else {
        const Vector2D uv = intersectionSector.getUVCoordinates(position);
        u = uv._x;
        v = uv._y;
    }
    
    //X and Y
    //const Vector2D uv = sector.getUVCoordinates(position);
    const long long x = mu->round( (u * _parameters->_tileTextureResolution._x) );
    const long long y = mu->round( (v * _parameters->_tileTextureResolution._y) );
    
    IStringBuilder* isb = IStringBuilder::newStringBuilder();
    isb->addString("&X=");
    isb->addLong(x);
    isb->addString("&Y=");
    isb->addLong(y);
    req += isb->getString();
    delete isb;
    
	return URL(req, false);
}

const std::string WMSLayer::description() const {
    return "[WMSLayer]";
}

bool WMSLayer::rawIsEquals(const Layer* that) const {
    WMSLayer* t = (WMSLayer*) that;
    
    if (!(_mapServerURL.isEquals(t->_mapServerURL))) {
        return false;
    }
    
    if (!(_queryServerURL.isEquals(t->_queryServerURL))) {
        return false;
    }
    
    if (_mapLayer != t->_mapLayer) {
        return false;
    }
    
    if (_mapServerVersion != t->_mapServerVersion) {
        return false;
    }
    
    if (_queryLayer != t->_queryLayer) {
        return false;
    }
    
    if (_queryServerVersion != t->_queryServerVersion) {
        return false;
    }
    
    if (!(_dataSector.isEquals(t->_dataSector))) {
        return false;
    }
    
    if (_format != t->_format) {
        return false;
    }
    
    if (_queryServerVersion != t->_queryServerVersion) {
        return false;
    }
    
    if (_srs != t->_srs) {
        return false;
    }
    
    if (_style != t->_style) {
        return false;
    }
    
    if (_isTransparent != t->_isTransparent) {
        return false;
    }
    
    if (_extraParameter != t->_extraParameter) {
        return false;
    }
    
    return true;
}

WMSLayer* WMSLayer::copy() const {
    return new WMSLayer(_mapLayer,
                        _mapServerURL,
                        _mapServerVersion,
                        _queryLayer,
                        _queryServerURL,
                        _queryServerVersion,
                        _dataSector,
                        _format,
                        _srs,
                        _style,
                        _isTransparent,
                        (_condition == NULL) ? NULL : _condition->copy(),
                        _timeToCache,
                        _readExpired,
                        (_parameters == NULL) ? NULL : _parameters->copy(),
                        _transparency,
                        _disclaimerInfo);
}

RenderState WMSLayer::getRenderState() {
    _errors.clear();
    if (_mapLayer.compare("") == 0) {
        _errors.push_back("Missing layer parameter: mapLayer");
    }
    const std::string mapServerUrl = _mapServerURL._path;
    if (mapServerUrl.compare("") == 0) {
        _errors.push_back("Missing layer parameter: mapServerURL");
    }
    if (_format.compare("") == 0) {
        _errors.push_back("Missing layer parameter: format");
    }
    
    if (_errors.size() > 0) {
        return RenderState::error(_errors);
    }
    return RenderState::ready();
}

const TileImageContribution* WMSLayer::rawContribution(const Tile* tile) const {
    
    const Tile* tileP = getParentTileOfSuitableLevel(tile); //Parent tile with a suitable Level
//    if (tile != tileP){
//        ILogger::instance()->logInfo("Fetching image for tile parent of level %d", tileP->_level);
//    }
    
    const Sector requestedImageSector = tileP->_sector;
    
    if (!_dataSector.touchesWith(requestedImageSector)) {
        return NULL;
    }
    else if (_dataSector.fullContains(requestedImageSector) && tile == tileP) {
        //Most common case tile of suitable level being fully coveraged by layer
        return ((_isTransparent || (_transparency < 1))
                ? TileImageContribution::fullCoverageTransparent(_transparency)
                : TileImageContribution::fullCoverageOpaque());
    }
    else {
        const Sector contributionSector = _dataSector.intersection(requestedImageSector);
        if (contributionSector.hasNoArea()){
            return NULL;
        }
        
        return ((_isTransparent || (_transparency < 1))
                ? TileImageContribution::partialCoverageTransparent(contributionSector, _transparency)
                : TileImageContribution::partialCoverageOpaque(contributionSector));
    }
}
