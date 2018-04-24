module Main where

import Data.WAVE
import Data.List
import Data.Word
import Data.Bits
import Data.Char
import Data.Maybe
import System.Environment
import System.Exit
import Data.Text (unpack)
import qualified Text.Builder as TB

main :: IO ()
main = do
    args <- getArgs
    let operation = case args of
                    [filename, name, rate] -> do return $ Just (getWAVEFile filename, name, rate)
                    _                      -> do putStrLn "Usage: pdm source.wav trackname samplerate"
                                                 exitFailure
                                                 return $ Nothing
    
    maybeWave <- operation
    let Just (iowave, name, rate) = maybeWave
    wave <- iowave
    let WAVE (WAVEHeader numChannels frameRate bitsPerSample maybeFrames) samples = wave
    
    let bitRate = fromIntegral $ frameRate * bitsPerSample
    let finalSampleRate = read rate :: Double

    putStrLn $ "//source sample rate " ++ show frameRate
    putStrLn $ "//source bits per sample " ++ show bitsPerSample
    putStrLn $ "//source bitrate " ++ show bitRate
    putStrLn $ "//num channels " ++ show numChannels
    putStrLn $ "//num frames " ++ show maybeFrames

    let source = map (\(ch1:rest) -> sampleToDouble ch1) samples

    putStrLn $ "//num source samples " ++ show (length source)

    let resampled = resample (fromIntegral frameRate) 16000 source

    putStrLn $ "//num resampled samples " ++ show (length resampled)

    let upscaled = resample 16000 (read rate) resampled

    putStrLn $ "//num upscaled samples " ++ show (length upscaled)

    let result = toCArray name . pack16 . pdm $ upscaled

    putStrLn result

toBit x = if x > 0
            then 1
            else 0

groups n = unfoldr (\xs -> if null xs then Nothing else Just (splitAt n xs))

resample :: Double -> Double -> [Double] -> [Double]
resample sourceRate targetRate samples = worker 0 0 samples
    where dec x = x - (fromIntegral (floor x) :: Double)
          ratio = sourceRate / targetRate
          worker pos count []       = []
          worker pos count [a]      = [a]
          worker pos count (a:b:as) = let newpos = pos + ratio
                                          interpolated = (a + (b - a) * (dec pos))
                                      in if count >= floor pos 
                                            then interpolated : worker newpos count (a:b:as)
                                            else worker pos (count+1) (b:as)

pdm :: [Double] -> [Word8]
pdm samples = drop 1 . map toBit . map fst . scanl (pdmStep) (0, 0) $ samples
    where pdmStep res x  = let qe = snd res
                               y = if x > qe
                                      then 1
                                      else -1
                           in (y, fromIntegral y - x + qe)


pack16 :: [Word8] -> [Word16]
pack16 bits = map (\bs -> foldl (\i (n, b) -> if b == 1 then setBit i n else i) 0 bs) . map (zip [0..15] . reverse) . groups 16 $ bits


toCArray :: String -> [Word16] -> String
toCArray name samples = unpack . TB.run $ start `mappend` (body samples `mappend` end)
    where   samplesLength = length samples
            start = TB.string $ "#ifndef TRACK" ++ nameUpper ++ "_H \n#define TRACK" ++ nameUpper ++ "_H \n \n#import <stdint.h>\n #define " ++ nameUpper ++ "_SIZE " ++ (show samplesLength) ++ "\nconst uint16_t track" ++ name ++ "[" ++ show samplesLength ++ "] = {"
            body (a:as) = foldl' (\s x -> s `mappend` ((TB.char ',') `mappend` (TB.string . show $ x))) (TB.string (show a)) as
            end = TB.string "}; \n#endif"  
            nameUpper = map toUpper name  
