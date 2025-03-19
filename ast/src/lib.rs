// Copyright © 2025 Jean Silva
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
//                            http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under
// the License.

//! # AST
//!
//! Library by which abstract syntax trees (ASTs) are generated for Helena files.

use std::{any::TypeId, sync::OnceLock};

use crate::{
  node::{NewlineNode, Node}
};

mod branch;
mod common;
mod function;
mod node;

enum Tree<T: Node> {
  Failed(),
  Successful(Vec<T>)
}

pub fn generate_ast(source: &str) -> Vec<impl Node> {
  max_leafing().get()
}

/// Obtains the maximum amount of appearances for each type of node as an independent, top-level,
/// leaf node in the tree. 0 in case a specific type of node cannot be a leaf, denoting that context
/// is required for its presence to be valid.
fn max_leafing<'a>() -> &'a [TypeId; 2] {
  static MAX_LEAFING: OnceLock<[TypeId; 2]> = OnceLock::new();
  MAX_LEAFING.get_or_init(|| {
    [
      TypeId::of::<NewlineNode<_, _>>(),
      TypeId::of::<FunctionNode>()
    ]
  })
}

#[cfg(test)]
mod tests {
  use crate::{function::FunctionNode, generate_ast};

  #[test]
  fn generates_ast_of_singly_lined_unparemeterized_empty_bodied_function_declaration() {
    let ast = generate_ast("func main()");
    assert_eq!(FunctionNode, ast);
  }
}
