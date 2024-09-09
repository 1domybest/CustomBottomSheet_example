//
//  BrightCoveRouter.swift
//  HypyG
//
//  Created by 온석태 on 1/29/24.
//

import Foundation
import Alamofire



enum BrightCoveRouter {
    case getStatus(BrightCoveGetStatusRequest)
}

extension BrightCoveRouter: APIRouterType {

    var baseURL: String {
        return CurrentServer().brightCoveUrl
    }

    var method: HTTPMethod {
        switch self {
        case .getStatus: return .get
        }
    }

    var path: String {
        switch self {
        case .getStatus: return "/jobs"
        }
    }

    var parameters: RequestParams {
        switch self {
        case .getStatus(let brightCoveGetStatusRequest): return .query(brightCoveGetStatusRequest)
        }
    }

}


struct BrightCoveGetStatusRequest: Codable {
    let jod_id : String
}


struct BrightCoveGetStatusResponse: Codable {
    let createdAt: String?
    let finishedAt: JSONNull?
    let id: String
    let privacy: Bool
    let state, ssaiState, sepState, submittedAt: String
    let test: Bool
    let updatedAt, region: String
    let reconnectTime: Int
    let liveStream, jobStatic, adInsertion, metadataPassthrough: Bool
    let outWorkerBytes, outWorkerBytesRate: Int
    let channelType: String
    let stateHistory: [StateHistory]
    let stateHistoryIsTruncated: Bool
    let jobVideocloudAssetID: String
    let liveDVRSlidingWindowDurationMS: Int
    let notifications: [String]
    let outputs: [Output]
    let videocloud: Videocloud
    let randomizeChunkURL: Bool
    let playbackURL, playbackURLDVR: String
    let encryption: Encryption
    let inputMediaFile: InputMediaFile
    let stream: Stream
    let outputMediaFiles: [OutputMediaFile]

    enum CodingKeys: String, CodingKey {
        case createdAt = "created_at"
        case finishedAt = "finished_at"
        case id, privacy, state
        case ssaiState = "ssai_state"
        case sepState = "sep_state"
        case submittedAt = "submitted_at"
        case test
        case updatedAt = "updated_at"
        case region
        case reconnectTime = "reconnect_time"
        case liveStream = "live_stream"
        case jobStatic = "static"
        case adInsertion = "ad_insertion"
        case metadataPassthrough = "metadata_passthrough"
        case outWorkerBytes = "out_worker_bytes"
        case outWorkerBytesRate = "out_worker_bytes_rate"
        case channelType = "channel_type"
        case stateHistory = "state_history"
        case stateHistoryIsTruncated = "state_history_is_truncated"
        case jobVideocloudAssetID = "job_videocloud_asset_id"
        case liveDVRSlidingWindowDurationMS = "live_dvr_sliding_window_duration_ms"
        case notifications, outputs, videocloud
        case randomizeChunkURL = "randomize_chunk_url"
        case playbackURL = "playback_url"
        case playbackURLDVR = "playback_url_dvr"
        case encryption
        case inputMediaFile = "input_media_file"
        case stream
        case outputMediaFiles = "output_media_files"
    }
}

// MARK: - Encryption
struct Encryption: Codable {
}

// MARK: - InputMediaFile
struct InputMediaFile: Codable {
    let audioBitrateInKbps, audioCodec, audioSampleRate, channels: JSONNull?
    let createdAt: String?
    let durationInMS: Int
    let errorClass, errorMessage, fileSizeBytes, finishedAt: JSONNull?
    let format, frameRate, height: JSONNull?
    let id: String
    let md5Checksum: JSONNull?
    let privacy: Bool
    let state: String
    let test: Bool
    let updatedAt: String
    let videoBitrateInKbps, videoCodec, width, totalBitrateInKbps: JSONNull?
    let url: JSONNull?

    enum CodingKeys: String, CodingKey {
        case audioBitrateInKbps = "audio_bitrate_in_kbps"
        case audioCodec = "audio_codec"
        case audioSampleRate = "audio_sample_rate"
        case channels
        case createdAt = "created_at"
        case durationInMS = "duration_in_ms"
        case errorClass = "error_class"
        case errorMessage = "error_message"
        case fileSizeBytes = "file_size_bytes"
        case finishedAt = "finished_at"
        case format
        case frameRate = "frame_rate"
        case height, id
        case md5Checksum = "md5_checksum"
        case privacy, state, test
        case updatedAt = "updated_at"
        case videoBitrateInKbps = "video_bitrate_in_kbps"
        case videoCodec = "video_codec"
        case width
        case totalBitrateInKbps = "total_bitrate_in_kbps"
        case url
    }
}

// MARK: - OutputMediaFile
struct OutputMediaFile: Codable {
    let audioBitrateInKbps: Double
    let audioCodec: String
    let audioSampleRate, channels: JSONNull?
    let createdAt: String?
    let durationInMS: Int
    let errorClass, errorMessage, fileSizeBytes, finishedAt: JSONNull?
    let format, fragmentDurationInMS, frameRate: JSONNull?
    let height: Int
    let id: String
    let md5Checksum: JSONNull?
    let privacy: Bool
    let rfc6381_AudioCodec, rfc6381_VideoCodec: JSONNull?
    let state: String
    let test: Bool
    let updatedAt: String
    let videoBitrateInKbps: Double
    let videoCodec, videoCodecProfile: String
    let width: Int
    let label: String
    let totalBitrateInKbps: Double
    let keyframeInterval: Int
    let keyframeIntervalFollowSource: Bool
    let segmentSeconds: Int
    let liveStream: Bool
    let playbackURL, playbackURLDVR, playbackURLVOD: String
    let playbackAddedCdns: [JSONAny]

    enum CodingKeys: String, CodingKey {
        case audioBitrateInKbps = "audio_bitrate_in_kbps"
        case audioCodec = "audio_codec"
        case audioSampleRate = "audio_sample_rate"
        case channels
        case createdAt = "created_at"
        case durationInMS = "duration_in_ms"
        case errorClass = "error_class"
        case errorMessage = "error_message"
        case fileSizeBytes = "file_size_bytes"
        case finishedAt = "finished_at"
        case format
        case fragmentDurationInMS = "fragment_duration_in_ms"
        case frameRate = "frame_rate"
        case height, id
        case md5Checksum = "md5_checksum"
        case privacy
        case rfc6381_AudioCodec = "rfc_6381_audio_codec"
        case rfc6381_VideoCodec = "rfc_6381_video_codec"
        case state, test
        case updatedAt = "updated_at"
        case videoBitrateInKbps = "video_bitrate_in_kbps"
        case videoCodec = "video_codec"
        case videoCodecProfile = "video_codec_profile"
        case width, label
        case totalBitrateInKbps = "total_bitrate_in_kbps"
        case keyframeInterval = "keyframe_interval"
        case keyframeIntervalFollowSource = "keyframe_interval_follow_source"
        case segmentSeconds = "segment_seconds"
        case liveStream = "live_stream"
        case playbackURL = "playback_url"
        case playbackURLDVR = "playback_url_dvr"
        case playbackURLVOD = "playback_url_vod"
        case playbackAddedCdns = "playback_added_cdns"
    }
}

// MARK: - Output
struct Output: Codable {
    let videoCodec: String
    let keyframeInterval: Int
    let liveStream: Bool
    let segmentSeconds, width: Int
    let h264Profile: String
    let rtmpCredentials: String?
    let label: String
    let videoBitrate: Int
    let notifications: [String]
    let height: Int

    enum CodingKeys: String, CodingKey {
        case videoCodec = "video_codec"
        case keyframeInterval = "keyframe_interval"
        case liveStream = "live_stream"
        case segmentSeconds = "segment_seconds"
        case width
        case h264Profile = "h264_profile"
        case rtmpCredentials = "rtmp_credentials"
        case label
        case videoBitrate = "video_bitrate"
        case notifications, height
    }
}

// MARK: - StateHistory
struct StateHistory: Codable {
    let state: String
    let transitionTimestamp: Int

    enum CodingKeys: String, CodingKey {
        case state
        case transitionTimestamp = "transition_timestamp"
    }
}

// MARK: - Stream
struct Stream: Codable {
    let createdAt: String?
    let finishedAt, height: JSONNull?
    let id, name: String
    let streamProtocol, state: JSONNull?
    let test: Bool
    let updatedAt: String
    let width, totalBitrateInKbps: JSONNull?
    let duration: Double
    let region, url: String
    let location: Location
    let inWorkerBytes, inWorkerBytesRate: Int

    enum CodingKeys: String, CodingKey {
        case createdAt = "created_at"
        case finishedAt = "finished_at"
        case height, id, name
        case streamProtocol = "protocol"
        case state, test
        case updatedAt = "updated_at"
        case width
        case totalBitrateInKbps = "total_bitrate_in_kbps"
        case duration, region, url, location
        case inWorkerBytes = "in_worker_bytes"
        case inWorkerBytesRate = "in_worker_bytes_rate"
    }
}

// MARK: - Location
struct Location: Codable {
    let source, destination: Destination
    let distance: JSONNull?
}

// MARK: - Destination
struct Destination: Codable {
    let latitude, longitude, location: JSONNull?
}

// MARK: - Videocloud
struct Videocloud: Codable {
    let video: Video
}

// MARK: - Video
struct Video: Codable {
    let name: String
}

// MARK: - Encode/decode helpers

class JSONNull: Codable, Hashable {

    public static func == (lhs: JSONNull, rhs: JSONNull) -> Bool {
        return true
    }

    public var hashValue: Int {
        return 0
    }

    public init() {}

    public required init(from decoder: Decoder) throws {
        let container = try decoder.singleValueContainer()
        if !container.decodeNil() {
            throw DecodingError.typeMismatch(JSONNull.self, DecodingError.Context(codingPath: decoder.codingPath, debugDescription: "Wrong type for JSONNull"))
        }
    }

    public func encode(to encoder: Encoder) throws {
        var container = encoder.singleValueContainer()
        try container.encodeNil()
    }
}

class JSONCodingKey: CodingKey {
    let key: String

    required init?(intValue: Int) {
        return nil
    }

    required init?(stringValue: String) {
        key = stringValue
    }

    var intValue: Int? {
        return nil
    }

    var stringValue: String {
        return key
    }
}

class JSONAny: Codable {

    let value: Any

    static func decodingError(forCodingPath codingPath: [CodingKey]) -> DecodingError {
        let context = DecodingError.Context(codingPath: codingPath, debugDescription: "Cannot decode JSONAny")
        return DecodingError.typeMismatch(JSONAny.self, context)
    }

    static func encodingError(forValue value: Any, codingPath: [CodingKey]) -> EncodingError {
        let context = EncodingError.Context(codingPath: codingPath, debugDescription: "Cannot encode JSONAny")
        return EncodingError.invalidValue(value, context)
    }

    static func decode(from container: SingleValueDecodingContainer) throws -> Any {
        if let value = try? container.decode(Bool.self) {
            return value
        }
        if let value = try? container.decode(Int64.self) {
            return value
        }
        if let value = try? container.decode(Double.self) {
            return value
        }
        if let value = try? container.decode(String.self) {
            return value
        }
        if container.decodeNil() {
            return JSONNull()
        }
        throw decodingError(forCodingPath: container.codingPath)
    }

    static func decode(from container: inout UnkeyedDecodingContainer) throws -> Any {
        if let value = try? container.decode(Bool.self) {
            return value
        }
        if let value = try? container.decode(Int64.self) {
            return value
        }
        if let value = try? container.decode(Double.self) {
            return value
        }
        if let value = try? container.decode(String.self) {
            return value
        }
        if let value = try? container.decodeNil() {
            if value {
                return JSONNull()
            }
        }
        if var container = try? container.nestedUnkeyedContainer() {
            return try decodeArray(from: &container)
        }
        if var container = try? container.nestedContainer(keyedBy: JSONCodingKey.self) {
            return try decodeDictionary(from: &container)
        }
        throw decodingError(forCodingPath: container.codingPath)
    }

    static func decode(from container: inout KeyedDecodingContainer<JSONCodingKey>, forKey key: JSONCodingKey) throws -> Any {
        if let value = try? container.decode(Bool.self, forKey: key) {
            return value
        }
        if let value = try? container.decode(Int64.self, forKey: key) {
            return value
        }
        if let value = try? container.decode(Double.self, forKey: key) {
            return value
        }
        if let value = try? container.decode(String.self, forKey: key) {
            return value
        }
        if let value = try? container.decodeNil(forKey: key) {
            if value {
                return JSONNull()
            }
        }
        if var container = try? container.nestedUnkeyedContainer(forKey: key) {
            return try decodeArray(from: &container)
        }
        if var container = try? container.nestedContainer(keyedBy: JSONCodingKey.self, forKey: key) {
            return try decodeDictionary(from: &container)
        }
        throw decodingError(forCodingPath: container.codingPath)
    }

    static func decodeArray(from container: inout UnkeyedDecodingContainer) throws -> [Any] {
        var arr: [Any] = []
        while !container.isAtEnd {
            let value = try decode(from: &container)
            arr.append(value)
        }
        return arr
    }

    static func decodeDictionary(from container: inout KeyedDecodingContainer<JSONCodingKey>) throws -> [String: Any] {
        var dict = [String: Any]()
        for key in container.allKeys {
            let value = try decode(from: &container, forKey: key)
            dict[key.stringValue] = value
        }
        return dict
    }

    static func encode(to container: inout UnkeyedEncodingContainer, array: [Any]) throws {
        for value in array {
            if let value = value as? Bool {
                try container.encode(value)
            } else if let value = value as? Int64 {
                try container.encode(value)
            } else if let value = value as? Double {
                try container.encode(value)
            } else if let value = value as? String {
                try container.encode(value)
            } else if value is JSONNull {
                try container.encodeNil()
            } else if let value = value as? [Any] {
                var container = container.nestedUnkeyedContainer()
                try encode(to: &container, array: value)
            } else if let value = value as? [String: Any] {
                var container = container.nestedContainer(keyedBy: JSONCodingKey.self)
                try encode(to: &container, dictionary: value)
            } else {
                throw encodingError(forValue: value, codingPath: container.codingPath)
            }
        }
    }

    static func encode(to container: inout KeyedEncodingContainer<JSONCodingKey>, dictionary: [String: Any]) throws {
        for (key, value) in dictionary {
            let key = JSONCodingKey(stringValue: key)!
            if let value = value as? Bool {
                try container.encode(value, forKey: key)
            } else if let value = value as? Int64 {
                try container.encode(value, forKey: key)
            } else if let value = value as? Double {
                try container.encode(value, forKey: key)
            } else if let value = value as? String {
                try container.encode(value, forKey: key)
            } else if value is JSONNull {
                try container.encodeNil(forKey: key)
            } else if let value = value as? [Any] {
                var container = container.nestedUnkeyedContainer(forKey: key)
                try encode(to: &container, array: value)
            } else if let value = value as? [String: Any] {
                var container = container.nestedContainer(keyedBy: JSONCodingKey.self, forKey: key)
                try encode(to: &container, dictionary: value)
            } else {
                throw encodingError(forValue: value, codingPath: container.codingPath)
            }
        }
    }

    static func encode(to container: inout SingleValueEncodingContainer, value: Any) throws {
        if let value = value as? Bool {
            try container.encode(value)
        } else if let value = value as? Int64 {
            try container.encode(value)
        } else if let value = value as? Double {
            try container.encode(value)
        } else if let value = value as? String {
            try container.encode(value)
        } else if value is JSONNull {
            try container.encodeNil()
        } else {
            throw encodingError(forValue: value, codingPath: container.codingPath)
        }
    }

    public required init(from decoder: Decoder) throws {
        if var arrayContainer = try? decoder.unkeyedContainer() {
            self.value = try JSONAny.decodeArray(from: &arrayContainer)
        } else if var container = try? decoder.container(keyedBy: JSONCodingKey.self) {
            self.value = try JSONAny.decodeDictionary(from: &container)
        } else {
            let container = try decoder.singleValueContainer()
            self.value = try JSONAny.decode(from: container)
        }
    }

    public func encode(to encoder: Encoder) throws {
        if let arr = self.value as? [Any] {
            var container = encoder.unkeyedContainer()
            try JSONAny.encode(to: &container, array: arr)
        } else if let dict = self.value as? [String: Any] {
            var container = encoder.container(keyedBy: JSONCodingKey.self)
            try JSONAny.encode(to: &container, dictionary: dict)
        } else {
            var container = encoder.singleValueContainer()
            try JSONAny.encode(to: &container, value: self.value)
        }
    }
}
