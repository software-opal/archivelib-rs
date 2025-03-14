use std::{collections::HashSet};
use rand::seq::SliceRandom;

fn is_without_runs(test: &[u8]) -> bool {
  let exp: usize = test.len() - 2;
  test.windows(3).collect::<HashSet<_>>().len() == exp
}

fn insert_all_without_runs(current_input: &[u8], possible_values: &[u8]) -> Option<Vec<u8>> {
  let mut possible_values = possible_values.to_owned();
  possible_values.shuffle(&mut rand::rng());
  return do_insert_all_without_runs(current_input, &possible_values);
}
fn do_insert_all_without_runs(current_input: &[u8], possible_values: &[u8]) -> Option<Vec<u8>> {
  let mut seen_poss_val = HashSet::new();

  let last_values = if current_input.len() >= 2 {
    Some((
      current_input[current_input.len() - 1],
      current_input[current_input.len() - 2],
    ))
  } else {
    None
  };
  let input_windows = current_input.windows(3).collect::<HashSet<_>>();

  for (idx, val) in possible_values.iter().enumerate() {
    if !seen_poss_val.insert(val) {
      continue;
    }
    let not_dup = if let Some((nm1, nm2)) = last_values {
      let new_last = [nm2, nm1, *val];
      !input_windows.contains(&new_last[..])
    } else {
      true
    };
    if not_dup {
      let mut new_input = current_input.to_owned();
      new_input.push(*val);
      let mut new_poss_val = possible_values.to_owned();
      new_poss_val.remove(idx);

      if new_poss_val.is_empty() {
        return Some(new_input);
      }
      if let Some(run) = do_insert_all_without_runs(&new_input, &new_poss_val) {
        return Some(run);
      }
    }
  }
  None
}
#[test]
fn test_insert_all_without_runs() {
  assert_eq!(
    do_insert_all_without_runs("".as_bytes(), "abcdd".as_bytes()),
    Some("abcdd".as_bytes().to_owned())
  );
  assert_witout_run(
    &insert_all_without_runs(
      "".as_bytes(),
      // "abcdddeeeffffffggggggggghhhhhhhhhhhhhhh".as_bytes()
      "
  abccdddeeeeeffffffffggggggggggggghhhhhhhhhhhhhhhhhhhhhiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii
  jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj
  kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk
  ABCCDDDEEEEEFFFFFFFFGGGGGGGGGGGGGHHHHHHHHHHHHHHHHHHHHHIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
  JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ
  KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
      "
      .replace("\n", "")
      .replace(" ", "")
      .as_bytes(),
    )
    .unwrap(),
  );
}

fn assert_witout_run(input: &[u8]) {
  eprintln!("{}", input.iter().map(|c| *c as char).collect::<String>());
  let mut seen_windows = HashSet::new();
  let input_str = input.iter().map(|c| *c as char).collect::<String>();
  for (i, window) in input.windows(3).enumerate() {
    if !seen_windows.insert(window) {
      panic!(
        "Window was seen twice at {}: {}\n{}",
        i,
        window.iter().map(|c| *c as char).collect::<String>(),
        input_str
      );
    }
  }
}

fn insert_without_runs(input: &[u8], c: char, count: usize) -> Vec<u8> {
  let mut input = input.iter().cloned().collect::<Vec<_>>();

  for i in 0..count {
    eprintln!(
      "i {}: {:#?}",
      i,
      input.iter().map(|c| *c as char).collect::<String>()
    );
    let mut did_insert = false;
    let mut curr_idx = 0;
    loop {
      let mut test = input.clone();
      test.insert(curr_idx, c as u8);
      let exp = test.len() - 2;
      if test.windows(3).collect::<HashSet<_>>().len() == exp {
        input.insert(curr_idx, c as u8);
        did_insert = true;
        break;
      }
      curr_idx += 1;
      if curr_idx > input.len() {
        break;
      }
    }
    if !did_insert {
      input = insert_without_runs(&input, c.to_ascii_uppercase(), 4);

      let mut did_insert = false;
      let mut curr_idx = 0;
      loop {
        let mut test = input.clone();
        test.insert(curr_idx, c as u8);
        let exp = test.len() - 2;
        if test.windows(3).collect::<HashSet<_>>().len() == exp {
          input.insert(curr_idx, c as u8);
          did_insert = true;
          break;
        }
        curr_idx += 1;
        if curr_idx > input.len() {
          break;
        }
      }
      if !did_insert {
        panic!()
      }
    }
  }
  input
}

#[test]
fn test_insert_p() {
  let data = "
    nEJDKHCnUmkooXJomZlnFoBonnOUmoRAGQoDoTRJoMPVnJoWnNnWVOlnlNJXBgUKTknjUVMIQBDVTlmZBJMUmSOoAGAmmmn
    oMkCToXUhGInomXkKOiYIEFSToCAZlmnnVonZPnoNVlHMGnECmnloolWRokKIDRomPDfDnonMiCBFSCXJOoSXSonoimlSiC
    NImPKnoRdkMnlloHjXZRjOPJGoQAMnoKMBCYCVGYMLNolhOnooWoILUFNPLlVoYLnolTPKTnoHoCHoAfVoHPmDJonPoonWn
    noZllMFVomhnVNomGoRZoFlBHGOSnSjOQRCJoYZoCSUjokolYomHGEZHonEhXmlWLSmoZVLKooSlUWYooLoooTWXomnBJNo
    TnnnDoAoRoioLFngQTnZlVnUlnNoAEGJmInlEoWTonOmnjooYnYnQUYSYoJYoKDQBImnSoBGkkAUDoWHVoNnmmUSoTNnGWY
    nnkhNLYYjESSfFWKRniYFBKGCJIDmoSohnQnkoBKoJSVFnAonIXoBNSoHnmLZmmWoWNoFoXAETPnLoOoIooNOllnPnjnokm
    NRITSVWFAMoBnGnoXNnoLEeFoPnHnihEEnWKNGTKXJnGYmlLnkRoPHLUnOLCIIniinoPZkPTOPBQlXYQnmOkSBnWUKChiLZ
    COooHWonBnnmooFKnQmkNoDGooOgkLKmnHmCnMlLXDUYLQFNCKPmFjFNnnPEmoIHWWloToJnoWmommonDMlMmnCnlYnoYMP
    OQAOIJionmnOSZTooPoONnNmnKoDnnCooIIoYAnmVoIPNmoloGmoXnlIUjDnXJlBlWokHnnJmkjnOoYmConVGoCoKonKmok
    JBDBoLZFolmoWXEjSDkonSnnRXVSSXoClnHKGnToEOkMQWRWJnIoVoBmnNRmjnQVTMbomJXlUABVnoGlTomFonlJoRYmmDn
    IWoBXMoEnjEVooRUYYonNLomoBLjlmGYKmLoNonUBlonjBHmQnoAJoCnnlnoFiVoVmKmOnTNooESWXnXQMgUPonJMomENoP
    GRZloZWZLIoJULZoXmJRoNmCMoMoVEKNBoYoYSomAooVnnTRoelXlAomMokUmARLVQCMmoJoUoBSkKnKQoOmMnWEoEonTDo
    oQnKWnmSnTEDlYAZiPoGOoDAoSnoSKlmNIoDQoZTHooMWooZHTQSooKnMookPRLnPOEZonCOnWomKoUnNMoInnQoomToGNm
    SJnJnRnHlAlINoWQHlnmlmmKOolCKXHZZnIYmoLmoHDZWoTAnoUldnLGnSkLknYDNBXAnLTQoYTjOMloDSoQYnCAoCYoUjn
    CGPiCGZCCYNSnFmoQXBoTZnXoJQoMOoOAoDHRiELOUGXLVGDLMnGoZCloBVmmNoVYnHOnUomVmoPQYmkToIEooBWoZImlFN
    lLoZoSDojRPGZLoFSKoOSClIoHBojnmMkomREDnMBoohKoXRUDATBkoIQlADlollVZooCRlSkXEkQWknoImoNlnRooUmEko
    klHFEEEKXlmITmHllAAMAoZYlnAFooinmGnVZKomOonXPmoTUNOTQhmoGnQRoLkEEOEnkTnWmVUooAloOBoSYnlWIomIHTo
    QlnnMnnXaomQomCOMfoFmTVoGIUnnFnoVGIPoAnnjkoNGonHolnVloLWoPloiZmDhnWNUGmnLOkmJQEVKUokjUFJWEGoPTo
    FIoEYkoAmoVHomkHoQIHoPAoQoTCoORoIZoJXoFDYmQFZEmIloJGLoGokZoVXoGRoAjoZnPjDJLoWFoRnnYoTOnVIOHFLCo
    ULXoSAlYDoKHoOPLUIjPmGHIoSZomDfnRJRYAHnoBOnNUoLnnIDoFEoJZoUKoEPoDYoQMonfmVKCLoKmXnnWFnmkmojoQmo
    KVolRjoNNmPoSWoXonkknUFBVoPBZiEomWRXCDSDmnGlAmPmKGKQGPmZFkWUSQFJgmRZnlLEnBoFnnHVRonQZolJCnoDkPn
    DmhojYMDWYNnlBoIYnmUnoQVoXMmmRnoEinFioDPomlomNnZoGAoPmnJloWRnBMoQOoQknngongMVoWkoTJoAUonAPBCmEn
    UnQEoLCnJTolKnJVnmEoMDonLEBkTRAoXXUZkWLWEHGgjlBjOJPmdoUOFAXoZmVHUoHGjoIKknClXoMNQgmnEOnhkJKnUVo
    knTLoPLojmoenTBoGZAVoJRloFToRKXDIEnXWKoNEoTmoAZoIhnnKIoIWmHoERoGJoSUADHAFSLmVOnCJMBFoABlPmjonRj
    jmmMmDoIRnQGiBomjKoTVmnmDmSoOnHFoGHoRXoojZoHFnPZoLcooJComBnoTkoWCnmHnPUnjNnUUoXHoLOoiInDnCTIonY
    loMIoZkoFJooDmmTnmKHWYHoXQoQFmmYmMlmEmmSUnASoFLHnBSQTCmFmBokkoJklOkDlNYnFQnRYnXZoAQoFUmJBPlRMCQ
    oKkoOYoAPJnMRMlVMnmRoCPnXHnCBoRGoNMHoBRoKCoAAoEMnREnnSPnnNHoMnDIoRMFoDCoPJlNDnlFoFXikiVKGoWlnjl
    oAToShonGBoXlnSmNYlOnF
  "
  .replace("\n", "")
  .replace(" ", "")
  .as_bytes()
  .to_owned();

  insert_without_runs(&data, 'p', 699);
}

#[test]
fn test_compressing_specially_crafted() {
  let input = {
    let mut start = 'c';
    let mut input = "ab".as_bytes().to_owned();
    let mut next_count = 2;
    // let mut start = 'j';
    // let mut input = "iiihihhiigihgighhfhigfihfiggiieghifiehgehfdhhdiiaiibiic".as_bytes().to_owned();
    // let mut next_count = 34;
    // for c in 'd'..='p' {
    for c in start..='p' {
      let this_count = next_count;
      next_count = input.len();
      next_count += 1;

      println!("c: {}; count: {}", c, this_count);
      input.append(&mut vec![c as u8; this_count]);
      eprintln!(
        "input {}: {:#?}",
        next_count,
        input.iter().map(|c| *c as char).collect::<String>()
      );
    }
    eprintln!(
      "input: {:#?}",
      input.iter().map(|c| *c as char).collect::<String>()
    );
    input
  };
  panic!();

  let result = archivelib::do_compress_level(&input, archivelib::CompressionLevel::Level0);
  let compressed_ab =
    hex!("00 12 3B A8 A2 1F FC 2E  40 37 8F A0 1A 39 27 2B  6F 86 41 95 97 BA 49 24  92 4B AF 00");
  assert_bytes_eq!(compressed_ab, &result.unwrap()[..])
}
